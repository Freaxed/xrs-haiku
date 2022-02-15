#ifndef _SFSTrack_H_
#define _SFSTrack_H_

#include "Track.h"
#include "Pattern.h"


#include "XFSynth.h"



class SFSTrack: public Track
{
	public:
	
						SFSTrack();
						~SFSTrack();
	virtual 	void			Reset();
	virtual	void			Process(float**,int32,int,float factor=1.0);
	TRACK_GEN_TYPE	getProcessorType() { return TT_FULL_PROCESS; }
	virtual	int			getModel();
	virtual	void			goOn();
	virtual	void			goOff();
	virtual 	void 			Message(SynthMessage msg, float data);

	virtual	bool			HasData(){ return hasdata; }
			void			SetHasData(bool b){ hasdata=b;}
	
	virtual	XRSVoice		newVoice(Note* n,int VoiceTag);
	virtual	void				stopVoice(int note=-1);
	virtual	void				RouteLineChanged(int line);
		

			void		SetChannel(int ch);
			int		GetChannel(){ return channel;}
			
			void		SetReverbSend(int val){ reverb_send=val;};
			int		GetReverbSend(){ return reverb_send;};
			
			void		SetChorusSend(int val){ chorus_send=val;};
			int		GetChorusSend(){ return chorus_send;};	
			virtual rgb_color	GetPreferredPadColor(){ rgb_color a={174,155,87,255}; return a;}


	void				SaveCustomSettings(BMessage& msg);
	void				LoadCustomSettings(BMessage& msg);

protected:
	virtual 	const char*		getPreferredName();	
		
		//float 		*stream_s[2];
		float 		*stream_l;
		float 		*stream_r;
		int			channel;
		bool			hasdata;
		
		int			reverb_send;
		int			chorus_send;


};

#endif
