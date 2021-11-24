/*

	XFSynth: direct wrap of the fluidsynth.

*/
#ifndef _XFSynth_H_
#define _XFSynth_H_

#include <Message.h>
#include <String.h>
//#include <ObjectList.h>

#include "fluidsynth.h"

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
			
			void	SaveChannelSettings(BMessage*,int);
			void	LoadChannelSettings(BMessage*,int);
			
			void	SaveGlobalSettings(BMessage*);
			void	LoadGloablSettings(BMessage*);
			
			
			fsynth	LoadFile(const char*);
			void	ApplySynth(fsynth);
			
			
			bool	IsValid(){ if(fSynth.synth) return true; else return false; } ;
			
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
			//bool	IsReverbOn();
			float	GetReverbRoomSize();
			float	GetReverbDamp();
			float	GetReverbWidth();
			float	GetReverbLevel();
			
			void	SetReverb(float roomsize,float damp,float width,float level);
			
			
			void	SetChorusOn(bool ch);
			//bool	IsChorusOn();
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
			
			
	private:		
			void	FreeSynth(fsynth&);
						
			fluid_settings_t* 	settings;
	
	private:
	friend class SFSPanel;
	friend class SFSTrackBoost;
	
			fsynth				fSynth;
			
};
#endif

//---
