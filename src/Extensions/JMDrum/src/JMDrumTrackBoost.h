#ifndef _JMDrumTrackBoost_H_
#define _JMDrumTrackBoost_H_

#include "TrackBoost.h"

class JMDrumTrackBoost : public TrackBoost
{
	public:
				  			 JMDrumTrackBoost();
							~JMDrumTrackBoost(){}
							
			 	Track*		getTrack();
			 	PlugPanel*	getPanel();

};
#endif

//--
