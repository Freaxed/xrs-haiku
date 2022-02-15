/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _VIW_TRACK_BOOST_H_
#define _VIW_TRACK_BOOST_H_

#include "TrackBoost.h"
#include <List.h>

class 	VIWTrack;
class	VIWPanel;
class   VSTPlugin;

class VIWTrackBoost : public TrackBoost
{
	public:
			 VIWTrackBoost();
			~VIWTrackBoost(){};
			
	Track*		getTrack();
	PlugPanel*	getPanel();

	VSTPlugin*	FindVSTi(const char* effectName);
	
	private:
			VIWPanel*	panel;			
			const BList*		list;
};
#endif
