#ifndef _MIDIOUT_TRACK_H_
#define _MIDIOUT_TRACK_H_

#include "Track.h"

class BMidiLocalProducer;

#include "midiout_locale.h"

#define	MAX_CC	3

class MidiOutTrack: public Track
{
	public:
	
				MidiOutTrack(BMidiLocalProducer*);
				~MidiOutTrack();
virtual 	void		Reset();
virtual	int		getModel();
virtual	void		goOn();
virtual	void		goOff();
virtual 	void 		Message(SynthMessage msg, float data);


	virtual	 void		setVolume(float val);
	virtual	 void		setPan(float val);
		
	void SetChannel(int n);
	int	GetChannel(){ return channel;};
	
	void SetCCValue(int n,int val);
	void SetCCNumber(int n,int num);
	
	int	GetCCValue(int n){return my_trk_contr[n].value;};
	int	GetCCNumber(int n){return my_trk_contr[n].number;};
	
	void SetProgramChange(int pg);
	int  GetProgramChange(){ return program_change;};
	
	
	
	//xrs2 (1.3)
	virtual	XRSVoice	newVoice(Note* n,int VoiceTag);
	virtual	void				stopVoice(int note=-1);
	
	void		SetSendOnPlay(bool b){ sendOnPlay=b;}
	bool		GetSendOnPlay(){ return sendOnPlay;};
virtual rgb_color	GetPreferredPadColor(){ rgb_color a={214,219,239,255}; return a;}
	
protected:
	virtual 	const char*	getPreferredName();
	
private:
	typedef struct trk_controller{
	
		int value;
		int number;
	
	};

	Pattern*	playingPattern;	
	Note*		curNote;
	BMidiLocalProducer *mider;
	int channel;
	time_t latency;
	int program_change;
	trk_controller	my_trk_contr[MAX_CC];
	bool	sendOnPlay;
};

#endif