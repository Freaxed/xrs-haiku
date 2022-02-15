/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TRACKMANAGER_
#define _TRACKMANAGER_

#include "Track.h"
#include "JTrack.h"

#include <View.h>

class TrackBoost;
class Song;
class TracksPanel;

class BMenu;
class BFile;

#define	MAX_PLUG	16 		//Only up to 16 plug_ins?

class TrackManager
{
public:
	
				~TrackManager();
				
	static TrackManager*	Get();	
	Track*			getTrack(int16);
	
	JTrack*			MakeJTrack(Track*, BRect rect, int16 n);		
	JTrack*			getJTrack();
	

	void			SaveBoosterSettings(BMessage& msg);
	void			LoadBoosterSettings(BMessage& msg);

	void			SaveBoosterSettings(int16 i, BMessage*);
	void			SaveTrackSettings(Track*,BMessage*);
	
	void			LoadBoosterSettings(BMessage*);
	void			LoadTrackSettings(Track*,BMessage*);
	
	
	Track*			SendRef(entry_ref,int16,BMessage *m=NULL);
	bool			RefReceived(entry_ref,JTrack*,BMessage *g=NULL);	
	bool			isBoosterValid(int);
	
	status_t		SelectTrack(JTrack* t);
	JTrack*			getCurrentJTrack(){ return curJTrack; };
	Track*			getCurrentTrack();
	void			soloTrack(Track*);
	
	void			getAllMyTrack(BList *l,int id);
	void			getAllMyJTrack(BList *l,int id);
	
	void			refreshAllMyTrack(int id);
	void			ResetPanel(Track*);
	void			RefreshSelected();
	void			ResetToSong(Song*);
	void			Init();
	//void			Restart();
	void			Close();
	
	void			GetXRSDirectoryEntry(entry_ref*,const char*);
	BMenu*			getMenu() {return myMenu;}
	
	void			ShowTrackInfo();
		
	protected:
	friend class TracksPanel;	
			
		TracksPanel*	curPanel;
	
	
	private:
					TrackManager();
		void		LoadAllTrackBoost();
		status_t	RegisterTrackBoost(TrackBoost* boost);
		
		//fix.. please rename these!!
		
		BMenu*		myMenu;
		TrackBoost* list[MAX_PLUG];	
		JTrack*		curJTrack;
		
		Song*		curSong;
		BView*		current;
};


#endif
