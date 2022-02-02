/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TN_TRACK_BOOST_H_
#define _TN_TRACK_BOOST_H_

#include "TrackBoost.h"
#include "TNPanel.h"

class TNTrackBoost : public TrackBoost
{
	public:
				 TNTrackBoost();
				~TNTrackBoost(){}
				
	PlugPanel* 	getPanel(){ return panel;}
	Track*		getTrack();
	
	bool		Load(Track* cur,int t,BFile* file,int32 va,int32 rt) ;
	void		SaveTrackSettings(Track*,BMessage*);
	void		LoadTrackSettings(Track* trk,BMessage* data);
	
	private:
		TNPanel	*panel;
};
#endif
