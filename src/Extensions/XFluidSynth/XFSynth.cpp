#include "XFSynth.h"

#include <Menu.h>
#include <MenuItem.h>
#include "sfs_locale.h"
#include "XHost.h"
#include "Log.h"
#include "LoadingError.h"

#define	MAXCHANNEL	16

#define	SYN fSynth.synth
#define IFS	if(fSynth.synth)


bool	logOnLoadingError = false;

void logFunc(int level, char *message, void *data)
{
	switch(level) {
		case FLUID_PANIC:
			LogFatal(message);
			if (logOnLoadingError)
				LoadingError::Add("SoundFont", message, "PANIC");
		break;
		case FLUID_ERR:
			LogError(message);
			if (logOnLoadingError)
				LoadingError::Add("SoundFont", message, "ERROR");
		break;
		case FLUID_WARN:
			LogInfo(message);
		break;
		case FLUID_DBG:
			LogDebug(message);
		break;
	};
}


XFSynth::XFSynth()
{
	fSynth.synth = NULL;
	fSynth.sfont=NULL;
	fSynth.sfont_id=-1;

	settings = NULL;
		
	fluid_set_log_function(FLUID_PANIC, logFunc, NULL);
	fluid_set_log_function(FLUID_ERR,   logFunc, NULL);
	fluid_set_log_function(FLUID_WARN,  logFunc, NULL);
	fluid_set_log_function(FLUID_DBG,   logFunc, NULL);
}


void
XFSynth::SaveChannelSettings(BMessage& data,int channel)
{
	if(!IsValid())
		return;
	
	fluid_preset_t* tmp=fluid_synth_get_channel_preset(fSynth.synth,channel);
	
	data.AddString("PresetName",tmp->get_name(tmp));
	data.AddInt16("PresetNum",tmp->get_num(tmp));
	data.AddInt16("PresetBankNum",tmp->get_banknum(tmp));
}

const char*	
XFSynth::GetSoundFontName()
{
	if(!IsValid())
		return T_SFS_NO_SF2;
		
	return fSynth.sfont->get_name(fSynth.sfont);
}

void	
XFSynth::SaveReverbSettings(BMessage& reverb)
{
	reverb.AddBool("On", IsReverbOn());
	reverb.AddFloat("RoomSize",GetReverbRoomSize());
	reverb.AddFloat("Damping",GetReverbDamp());
	reverb.AddFloat("Width",GetReverbWidth());
	reverb.AddFloat("Level",GetReverbLevel());	
}

void	
XFSynth::SaveChorusSettings(BMessage& chorus)
{
	chorus.AddBool("On", IsChorusOn());
	chorus.AddInt16("Nr",GetChorusNr());
	chorus.AddFloat("Level",GetChorusLevel());
	chorus.AddFloat("Speed",GetChorusSpeed());
	chorus.AddFloat("Depth",GetChorusDepth());
	chorus.AddInt16("Type", GetChorusType());
}


void
XFSynth::SaveSettings(BMessage& data)
{
	if(!IsValid())
		return;
		
	data.AddString("SoundFont", GetSoundFontName());
	data.AddFloat("Gain", GetGain());

	for (int i=0;i<MAXCHANNEL;i++) 
	{
		BMessage channel;
		SaveChannelSettings(channel, i);
		data.AddMessage("Channel", &channel);
	}

	BMessage reverb;
	SaveReverbSettings(reverb);
	data.AddMessage("Reverb", &reverb);

	BMessage chorus;
	SaveChorusSettings(chorus);	
	data.AddMessage("Chorus", &chorus);
}
void
XFSynth::LoadChannelSettings(BMessage& data,int channel)
{
	if(!IsValid())
		return;

	int16 bankNum = data.GetInt16("PresetBankNum", 0);
	int16 presNum = data.GetInt16("PresetNum", 0);
	
	ProgramSelect(channel, bankNum, presNum);
}

bool	
XFSynth::LoadSoundFont(const char* filename)
{
	//
	Reset();
	//

	fsynth newsynth = LoadFile(filename);
	
	XHost::Get()->SendMessage(X_LockSem,0);
		
	ApplySynth(newsynth);
	
	XHost::Get()->SendMessage(X_UnLockSem,0);

	return !(newsynth.sfont_id == -1);
	
}

void	
XFSynth::LoadReverbSettings(BMessage& reverb)
{
	// Reverb
	bool  revOn = reverb.GetBool("On", 			false);
	float revRs = reverb.GetFloat("RoomSize", 	FLUID_REVERB_DEFAULT_ROOMSIZE);
	float revDa = reverb.GetFloat("Damping", 	FLUID_REVERB_DEFAULT_DAMP);
	float revWi = reverb.GetFloat("Width", 		FLUID_REVERB_DEFAULT_WIDTH);
	float revLe = reverb.GetFloat("Level", 		FLUID_REVERB_DEFAULT_LEVEL);	
	
	SetReverbOn(revOn);
	SetReverb(revRs, revDa, revWi, revLe);
}

void	
XFSynth::LoadChorusSettings(BMessage& chorus)
{
	bool  choOn = chorus.GetBool("On",     false);
	int   choNr = chorus.GetInt16("Nr",    FLUID_CHORUS_DEFAULT_N);
	float choLe = chorus.GetFloat("Level", FLUID_CHORUS_DEFAULT_LEVEL);
	float choSp = chorus.GetFloat("Speed", FLUID_CHORUS_DEFAULT_SPEED);
	float choDe = chorus.GetFloat("Depth", FLUID_CHORUS_DEFAULT_DEPTH);
	int   choTy = chorus.GetInt16("Type",  FLUID_CHORUS_MOD_SINE);

	SetChorusOn(choOn);
	SetChorus(choNr, choLe, choSp, choDe, choTy);
}
			

void
XFSynth::LoadSettings(BMessage& data)
{
	logOnLoadingError = true;
	const char* filename = data.GetString("SoundFont", "");
	if (!LoadSoundFont(filename))
	{
		BString what("Can't load the SoundFont file [");
		what << filename << "]!";
		LoadingError::Add("SoundFont", what.String(), "Find the missing SoundFont and assign to the right tracks!");
	}

	//DumpPreset();

	int i=0;
	BMessage channel;
	while(data.FindMessage("Channel", i, &channel) == B_OK)
	{
		LoadChannelSettings(channel, i);
		i++;
	}
	
	SetGain(data.GetFloat("Gain", 0.8f));	
	
	BMessage reverb;
	data.FindMessage("Reverb", &reverb);
	LoadReverbSettings(reverb);
	
	BMessage chorus;
	data.FindMessage("Chorus", &chorus);
	LoadChorusSettings(chorus);	

	logOnLoadingError = false;
}
			

XFSynth::~XFSynth()
{
	FreeSynth(fSynth);
	if(settings)
		delete_fluid_settings(settings);
}

void
XFSynth::Init()
{
	settings = new_fluid_settings();
	fluid_settings_setnum(settings, (char*)"synth.sample-rate", 44100.0);
	fluid_settings_setint(settings, (char*)"synth.polyphony",   256);
}

void
XFSynth::Reset(){
	//free the current synth
	FreeSynth(fSynth);
}

void	
XFSynth::FreeSynth(fsynth& synth) {

	if(synth.sfont_id >-1) 
		fluid_synth_sfunload(synth.synth,synth.sfont_id,1);

	if(synth.synth)
		delete_fluid_synth(synth.synth);
	
	//if(synth.sfont)
	//	delete synth.sfont;
		
	synth.sfont_id = -1;
	synth.sfont = NULL;
	synth.synth = NULL;

}


fsynth
XFSynth::LoadFile(const char* filename)
{
	fsynth	newsynth;
	
	newsynth.synth    = NULL;
	newsynth.sfont    = NULL;
	newsynth.sfont_id = -1;
	
	if(!filename || !settings) return newsynth;
	
	
	newsynth.synth 		= new_fluid_synth(settings);
	newsynth.sfont_id	= fluid_synth_sfload(newsynth.synth, filename, 1);
	
	if(newsynth.sfont_id == -1) {
		
		delete_fluid_synth(newsynth.synth);
		newsynth.synth    = NULL;
		newsynth.sfont    = NULL;
		newsynth.sfont_id = -1;
		return newsynth;
	}
	
	newsynth.sfont = fluid_synth_get_sfont_by_id(newsynth.synth,newsynth.sfont_id);
	
	for(int i=0;i<MAXCHANNEL;i++)
		fluid_synth_sfont_select(newsynth.synth,i,newsynth.sfont_id);
		
	fluid_synth_system_reset(newsynth.synth);
	fluid_synth_program_reset(newsynth.synth);
		
	return newsynth;
}

void	
XFSynth::ApplySynth(fsynth newsynth) {

	if(	newsynth.synth == NULL || 
	    newsynth.sfont == NULL || 
		newsynth.sfont_id == -1 )
	return;
			
	fsynth oldsynth;
	oldsynth.synth = fSynth.synth;
	oldsynth.sfont = fSynth.sfont;
	oldsynth.sfont_id = fSynth.sfont_id;
	
	fSynth.synth 	 = newsynth.synth;
	fSynth.sfont_id  = newsynth.sfont_id;
	fSynth.sfont	 = newsynth.sfont;

	FreeSynth(oldsynth);
}


void	
XFSynth::NoteOff(int channel,int note){
	
	if(fSynth.synth)
		fluid_synth_noteoff(fSynth.synth, channel, note);
}

void	
XFSynth::NoteOn(int channel,int note,int vol){
	
	if(fSynth.synth)
		fluid_synth_noteon(fSynth.synth, channel, note, vol);
}


void	
XFSynth::WriteFloat(uint64 frames,float *l,float* r){
	if(fSynth.synth)
		fluid_synth_write_float(fSynth.synth,frames,l,0,1,r,0,1);
}

void	
XFSynth::DumpPreset()
{
	int bank = -1;
	int ret = 1;
	
	if(!fSynth.synth || !fSynth.sfont) return;
	
	fluid_preset_t* res = fSynth.sfont->get_preset(fSynth.sfont,0,0);
	
	BString s;
	fSynth.sfont->iteration_start(fSynth.sfont);
	while(ret > 0)
	{
		ret = fSynth.sfont->iteration_next(fSynth.sfont, res);
		//printf("preset name %s bank %d, num %d \n",res->get_name(res),res->get_banknum(res),res->get_num(res));
		
		if(res->get_banknum(res)!=bank)
		{
			//new Bank!
			bank=res->get_banknum(res);
			
			s.SetTo("bank ");
			s << bank;
			LogTrace("Bank: %s", s.String());
		}

		LogTrace("get_name %s", res->get_name(res));
	}	
}

void
XFSynth::FillMenu(BMenu* menu) {

	int bank=-1;
	int ret=1;
	fluid_preset_t* res;
	BMessage *info;
	BMenu*	menu_bank=NULL;	
	
	
	//clean up the menu
		
	
	if(!fSynth.synth || !fSynth.sfont) return;
	
	res= fSynth.sfont->get_preset(fSynth.sfont,0,0);
	
	BString s;
	fSynth.sfont->iteration_start(fSynth.sfont);
	while(ret>0)
	{
		ret=fSynth.sfont->iteration_next(fSynth.sfont,res);
		//printf("preset name %s bank %d, num %d \n",res->get_name(res),res->get_banknum(res),res->get_num(res));
		
		if(res->get_banknum(res)!=bank)
		{
			//new Bank!
			bank=res->get_banknum(res);
			
			s.SetTo("bank ");
			s << bank;
			menu_bank=new BMenu(s.String());
			menu->AddItem(menu_bank);
			
		}
		info=new BMessage(BANK_PRESET);
		info->AddInt16("bank",bank);
		info->AddInt16("num",res->get_num(res));
		menu_bank->AddItem(new BMenuItem(res->get_name(res),info));
	}	
	

	//end

}

//effects
void 	XFSynth::SetGain(float g) { IFS fluid_synth_set_gain(SYN,g); };
float	XFSynth::GetGain() { IFS return fluid_synth_get_gain(SYN); else return 0; };

void	XFSynth::SetReverbOn(bool rev){ IFS fluid_synth_set_reverb_on(SYN,rev); fReverbOn = rev;};
bool	XFSynth::IsReverbOn(){ return fReverbOn;};

float	XFSynth::GetReverbRoomSize(){ IFS return fluid_synth_get_reverb_roomsize(SYN); else return 0;}
float	XFSynth::GetReverbDamp(){ IFS return fluid_synth_get_reverb_damp(SYN); else return 0;}
float	XFSynth::GetReverbWidth(){ IFS return fluid_synth_get_reverb_width(SYN); else return 0;}
float	XFSynth::GetReverbLevel(){ IFS return fluid_synth_get_reverb_level(SYN); else return 0;}

void	XFSynth::SetReverb(float roomsize,float damp,float width,float level)
		{ IFS fluid_synth_set_reverb(SYN,
				roomsize,
				damp,
				width,
				level);
		}
		
void	XFSynth::SetChorusOn(bool ch){ IFS fluid_synth_set_chorus_on(SYN,ch); fChorusOn = ch;};
bool	XFSynth::IsChorusOn() { return fChorusOn;};
int		XFSynth::GetChorusType(){ IFS return fluid_synth_get_chorus_type(SYN); else return 0;}
float	XFSynth::GetChorusLevel(){ IFS return fluid_synth_get_chorus_level(SYN); else return 0;}
float	XFSynth::GetChorusDepth(){ IFS return fluid_synth_get_chorus_depth_ms(SYN); else return 0;}
int		XFSynth::GetChorusNr(){ IFS return fluid_synth_get_chorus_nr(SYN); else return 0;}
float	XFSynth::GetChorusSpeed(){ IFS return fluid_synth_get_chorus_speed_Hz(SYN); else return 0;}
			
void	XFSynth::SetChorus(int nr,float level,float speed,float depth,int type)
		{ IFS 	fluid_synth_set_chorus(SYN,
				nr,
				level,
				speed,
				depth,
				type);
		}
		
const char*	
XFSynth::GetChannelPresetName(int channel) const {
	IFS 
	return fluid_synth_get_channel_preset(SYN,0)->get_name(fluid_synth_get_channel_preset(SYN,channel));
	else
	return NULL;
}

void		
XFSynth::ProgramSelect(int channel,int bank,int num) {
	IFS
	fluid_synth_program_select(SYN,channel,fSynth.sfont_id,bank,num);
				
}

void		
XFSynth::SendCC(int chan, int ctrl, int val) {
	IFS
	fluid_synth_cc(SYN, chan,ctrl,val);
}

