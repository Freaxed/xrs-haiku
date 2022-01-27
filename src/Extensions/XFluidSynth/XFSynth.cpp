#include "XFSynth.h"

#include <Menu.h>
#include <MenuItem.h>

#include "Log.h"

#define	MAXCHANNEL	16

#define	SYN fSynth.synth
#define IFS	if(fSynth.synth)

void logFunc(int level, char *message, void *data)
{
	switch(level) {
		case FLUID_PANIC:
			LogFatal(message);
		break;
		case FLUID_ERR:
			LogError(message);
		break;
		case FLUID_WARN:
			LogInfo(message);
		break;
		case FLUID_DBG:
			LogDebug(message);
		break;
	};
}


XFSynth::XFSynth(){

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
XFSynth::SaveChannelSettings(BMessage* data,int channel){

	if(!fSynth.synth) return;
	
	fluid_preset_t* tmp=fluid_synth_get_channel_preset(fSynth.synth,channel);
	
	data->AddString("preset_name",tmp->get_name(tmp));
	data->AddInt16("preset_num",tmp->get_num(tmp));
	data->AddInt16("preset_banknum",tmp->get_banknum(tmp));

}

void
XFSynth::LoadChannelSettings(BMessage* data,int channel){

	if(!fSynth.synth) return;
	fluid_synth_program_select(fSynth.synth,channel,0,data->FindInt16("preset_banknum"),data->FindInt16("preset_num"));


}

void
XFSynth::SaveGlobalSettings(BMessage* data){

	if(fSynth.sfont)
		data->AddString("bank_name",fSynth.sfont->get_name(fSynth.sfont));

}

void
XFSynth::LoadGloablSettings(BMessage* data){

}
			

XFSynth::~XFSynth(){

	FreeSynth(fSynth);

	if(settings)
		delete_fluid_settings(settings);
	
}

void
XFSynth::Init(){

	settings = new_fluid_settings();
	fluid_settings_setnum(settings, (char*)"synth.sample-rate", 44100.0);
	fluid_settings_setint(settings, (char*)"synth.polyphony", 256);
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
	
	newsynth.synth=NULL;
	newsynth.sfont=NULL;
	newsynth.sfont_id=-1;
	
	if(!filename || !settings) return newsynth;
	
	
	newsynth.synth 		= new_fluid_synth(settings);
	newsynth.sfont_id	= fluid_synth_sfload(newsynth.synth,filename,1);
	
	if(newsynth.sfont_id < 0) {
		
		delete_fluid_synth(newsynth.synth);
		newsynth.synth=NULL;
		newsynth.sfont=NULL;
		newsynth.sfont_id=-1;
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

	if(	newsynth.synth == NULL || newsynth.sfont == NULL || newsynth.sfont_id == -1 ) return;
			
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

void	XFSynth::SetReverbOn(bool rev){ IFS fluid_synth_set_reverb_on(SYN,rev); };
//bool	XFSynth::IsReverbOn(){ IFS return fluid_synth_is_reverb_on(SYN); else return false;};

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
		
void	XFSynth::SetChorusOn(bool ch){ IFS fluid_synth_set_chorus_on(SYN,ch); };
//bool	IsChorusOn();
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

