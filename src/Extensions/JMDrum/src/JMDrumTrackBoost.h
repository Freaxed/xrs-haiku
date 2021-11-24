#ifndef _JMDrumTrackBoost_H_
#define _JMDrumTrackBoost_H_

#include "TrackBoost.h"

class JMDrumTrackBoost : public TrackBoost
{
	public:
			JMDrumTrackBoost();
			virtual		~JMDrumTrackBoost(){}
	virtual 	Track*		getTrack();
	virtual 	PlugPanel*	getPanel();
	
	virtual	void			SaveTrackSettings(Track*,BMessage*);
	virtual	void			LoadTrackSettings(Track* trk,BMessage* data);

};
#endif

//--
