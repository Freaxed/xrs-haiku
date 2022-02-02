/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TRACKLIST_H_
#define _TRACKLIST_H_

#include <View.h>

#include "TrackEnd.h"
#include "Vector.h"

class TrackBlock;

class TrackList : public BView
{
	public:
			     	 TrackList(BRect r);
			     	 
	virtual void 	 MessageReceived(BMessage*);
	        void	 AddTrack(TrackBlock*);
			void	 RemoveTrack(TrackBlock*);
			void	 Expanded(TrackBlock*,float );
			void	 UnExpanded(TrackBlock*,float);
	private:
		 Vector<TrackBlock*>	list;
				
};
#endif
