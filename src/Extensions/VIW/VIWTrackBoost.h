/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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

class VIWTrackBoost : public TrackBoost
{
	public:
			 VIWTrackBoost();
			~VIWTrackBoost(){ delete list;}
	Track*		getTrack();
	PlugPanel*	getPanel();
	
	
	void			SaveTrackSettings(Track*,BMessage*);
	void			LoadTrackSettings(Track* trk,BMessage* data);
	
	private:
			VIWPanel*	panel;
			int			FindVSTi(const char* leaf);
			BList*		list;
};
#endif
