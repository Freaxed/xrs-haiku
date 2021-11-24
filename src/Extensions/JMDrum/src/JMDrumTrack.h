#ifndef _JMDrumTrack_H_
#define _JMDrumTrack_H_

#include "Track.h"
#include "Pattern.h"

#include "Drum.h"

class JMDrumPanel;
class JMDrumTrackBoost;

class JMDrumTrack: public Track
{
	public:
	
						JMDrumTrack();
						~JMDrumTrack();
	virtual 	void			Reset();
	virtual 	int32		ProcessVoice(XRSVoice,float ** buffer ,int32 sample_num); //,int spiaz);
	virtual	void			killVoice(XRSVoice);
	virtual 	int			getProcessorType() { return 1; } //0=full process ; 1=voice process
	
	virtual	int			getModel();
	virtual	void			goOn();
	virtual	void			goOff();
	virtual 	void 			Message(SynthMessage msg, float data);

	virtual	bool			SupportPanNote(){ return true; }; //tmp
	virtual rgb_color	GetPreferredPadColor(){ rgb_color a={166,164,188,255}; return a;}

	virtual	XRSVoice		newVoice(Note* n,int VoiceTag);
	virtual	void				stopVoice(int note=-1);
	
protected:
		virtual 	const char*	getPreferredName();
protected:
friend class JMDrumPanel;
friend class JMDrumTrackBoost;		

		DrumPars	values;	//internal values
		int*			Vals;	//slider values	
};

#endif
//--
