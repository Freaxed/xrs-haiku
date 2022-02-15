/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _VIWTrack_H_
#define _VIWTrack_H_

#include "Track.h"
#include "Pattern.h"

class		VSTInstrumentPlugin;
class		PlugWindow;
class 		VSTPlugin;
class		VIWTrackBoost;


class VIWTrack: public Track
{
	public:
	
						VIWTrack(VIWTrackBoost* );
						~VIWTrack();
	void			Reset();
	void			Process(float**,int32,int,float factor=1.0);
	TRACK_GEN_TYPE	getProcessorType() { return TT_FULL_PROCESS; }
	
	int				getModel();
	void			goOn();
	void			goOff();
	void 			Message(SynthMessage msg, float data);
	XRSVoice		newVoice(Note* n,int VoiceTag);
	void			stopVoice(int note=-1);
	
	
	VSTInstrumentPlugin*		getViw(){return viw;};
	PlugWindow*					getWin(){return win;};
	void						setWin(PlugWindow* w){win=w;};
	void						LoadVSTi(VSTPlugin* plugin);

 rgb_color	GetPreferredPadColor(){ rgb_color a={255,152,49,255}; return a;}
	
	
						
	void				SaveCustomSettings(BMessage& msg);
	void				LoadCustomSettings(BMessage& msg);
	
protected:
			const char*	getPreferredName();	
private:
		
		
		float 		*stream_s[2];
		
		VSTInstrumentPlugin*	viw;
		PlugWindow*				win;
		VIWTrackBoost*			mBoost;
};

#endif
