/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#ifndef _TRACK_H_
#define _TRACK_H_

#include "GlobalDef.h"
#include "synthMessages.h"
#include "XRSVoice.h"
#include "StereoVolumes.h"

#include <GraphicsDefs.h>
#include <SupportDefs.h>
#include <List.h>
#include <String.h>

class Pattern;
class Note;

enum TRACK_GEN_TYPE {
	TT_FULL_PROCESS  = 0,
	TT_VOICE_PROCESS = 1	
};


class Track: public StereoVolumes
{
	public:
	
				   Track();
		virtual	~Track();
		
	
		virtual	Pattern*	getCurrentPattern();		
		virtual void		goOn(){}
		virtual void		goOff(){}
		virtual	void		Reset();
		virtual	void 		Message(SynthMessage msg, float data);
		virtual	void		Process(float**,int32,int,float factor=1.0);
		virtual int			getModel();	
		
		// new in Xrs1.3
		
		virtual	TRACK_GEN_TYPE	getProcessorType() { return TT_VOICE_PROCESS; }
		virtual	XRSVoice		newVoice(Note* n,int VoiceTag){ return NULL; };		
		/*
			sample_num è il numero di full_frame
			se è 4, vuol dire che ci sono 4 frame per ogni canale.
			restituisce il numero di full frame scritti.
		*/
		virtual uint32		ProcessVoice(XRSVoice,float ** buffer ,uint32 sample_num){ return -1; };
		virtual	void		killVoice(XRSVoice){};
		virtual	void		stopVoice(int num=-1){};
		
		//New in XRS 1.4

		virtual	bool			HasData(){ return true;}; //may I ask 'process' ?
		virtual	void			RouteLineChanged(int line){};
		
		//tmp
		virtual	bool			SupportPanNote(){ return false; };
		virtual	bool			SupportSustainNote(){ return false;};
		
		//Measures

		Pattern*		getPatternAt(int);
		void				moveToPattern(int);
		
		bool				isOn();
		void				setOn(bool);
		int				getRouteLine(){ return route_line; };
		void				setRouteLine(int r){ route_line=r;};
		
		void				AddMeasure(int val,int);
		void				RemoveMeasure(int num);
		void				setName(const char*);
		void				LockName(bool b);
		const char*		getName();
		void				ResetName();
		bool				isNameLocked();
		void				setNumberNotes(int);
		
		BList				voice_list;	//Playing note list 
		bool				cit; 		//Cut_It_Self		
		
				
		
		//Midi-In
		bool		IsMidiInEnable() { return	midiin_enable;}
		void		SetMidiInEnable(bool val) { midiin_enable=val;}
		int			GetMidiInCh(){ return midiin_channel;}
		void		SetMidiInCh(int ch){ midiin_channel=ch;}
		
		//new in 1.4
		virtual rgb_color	GetPreferredPadColor(){ rgb_color a={98,176,137,255}; return a;}
	
protected:

		Note*				curNote;
		Pattern*			currentPattern;
		BList				patternList;
		virtual 	const char*	getPreferredName();	
		
			
private:
	
		BString			name;
		bool			name_set;
		bool			on;
		int 			route_line;
		
		//MIDI
		int				midiin_channel;
		bool			midiin_enable;
		
};
#endif
