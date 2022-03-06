/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef  _TRACKS_PANEL_
#define  _TRACKS_PANEL_

#include <View.h>
#include <List.h>

#include "GlobalDef.h"
#include "TrackList.h"
#include "ColorsDef.h"
#include "TrackManager.h"
#include "VectorNew.h"

class Song;
class Pattern;
class JTrack;
class Track;


class TracksPanel : public TrackList
{
	public:
						TracksPanel (BRect);
						
		 	void		MessageReceived(BMessage* message);
		 	void		MouseDown(BPoint);
			void		FrameResized(float new_w,float new_h);
			void		ScrollTo(BPoint p);
		
			status_t		Init(BView*);
				void		ResetToSong(Song *);
				int			getNumberJTrack();			
				void		RemoveTrackAt(int id);
				void		RemoveTrack(int);
				void		RenameSelectedTrack();
			 Track*			getCurrentTrack();
				void		SelectTrack(int);
				void		AddTrack(int h);
				
				void		resetPattern();
				int			getCurrentPattern();
				
								
				void		muteSelectedTrack();
				void		soloSelectedTrack();	
					
	protected:
	friend class TrackManager;
						JTrack*	getJTrackAt(int h);		
						void		RefreshGraphics();
						void		RefreshSelected();
						void		Refresh(JTrack* t);
	private:
	
		
		void				FixScrollBar();
		
		VectorNew<JTrack*>	xnv;
		Song			*curSong;
		TrackManager	*tm; //uhm shold be removed for sanity
		BView			*ticks;	
		
};

#endif

