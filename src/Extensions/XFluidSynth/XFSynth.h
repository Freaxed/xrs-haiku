/*

	XFSynth: direct wrap of the fluidsynth.

*/
#ifndef _XFSynth_H_
#define _XFSynth_H_

#include <Message.h>
#include <String.h>

#include <fluidlite.h>

#define	BANK_PRESET	'pres'

typedef struct {
			
	fluid_synth_t* 		synth;
	fluid_sfont_t*		sfont;
	int					sfont_id;	

} fsynth;
			

class SFSPanel; //FIX! (remove)
class SFSTrackBoost; //FIX REMOVE
class BMenu;

class XFSynth {

	public:
			 XFSynth();
			~XFSynth();
			
			void	Init();		//only once
			void	Reset();	//clean up all
			
			void	SaveSettings(BMessage&);
			void	LoadSettings(BMessage&);
			
			

			
			
			bool	IsValid(){ return (fSynth.synth != NULL); } ;
			
			//dsp
			void	NoteOff(int channel,int note);
			void	NoteOn(int channel,int note,int vol);
			
			void	WriteFloat(uint64,float*,float*);
			
			
			//effect
			
			//gain
			void	SetGain(float g);
			float	GetGain();
			
			//reverb
			void	SetReverbOn(bool rev);
			bool	IsReverbOn();
			
			float	GetReverbRoomSize();
			float	GetReverbDamp();
			float	GetReverbWidth();
			float	GetReverbLevel();
			
			void	SetReverb(float roomsize,float damp,float width,float level);
			
			
			void	SetChorusOn(bool ch);
			bool	IsChorusOn();
			
			int		GetChorusType();
			float	GetChorusLevel();
			float	GetChorusDepth();
			int		GetChorusNr();
			float	GetChorusSpeed();
			
			void	SetChorus(int nr,float level,float speed,float depth,int type);
						
			
			//channel preset
			const char*	GetChannelPresetName(int channel) const;
			void		ProgramSelect(int channel,int bank,int num);
			
			void		SendCC(int chan, int ctrl, int val);
			//misc..
			
			void	FillMenu(BMenu*);
			
			const char*	GetSoundFontName();
					
			bool	LoadSoundFont(const char* filename);
			
	private:		
			void	DumpPreset();

			fsynth	LoadFile(const char*);
			void	ApplySynth(fsynth);
		
			void	SaveChannelSettings(BMessage&,int);
			void	LoadChannelSettings(BMessage&,int);

			void	SaveReverbSettings(BMessage&);
			void	LoadReverbSettings(BMessage&);

			void	SaveChorusSettings(BMessage&);
			void	LoadChorusSettings(BMessage&);
			
			void	FreeSynth(fsynth&);
						
			fluid_settings_t* 	settings;
	
	private:
	friend class SFSPanel;
	friend class SFSTrackBoost;
	
			fsynth				fSynth;
			
			bool				fReverbOn;
			bool				fChorusOn;
			
};
#endif

//---
