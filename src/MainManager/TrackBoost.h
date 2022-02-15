/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#ifndef _TRACK_BOOST_H_
#define _TRACK_BOOST_H_

#include <View.h>
#include <Entry.h>
#include <String.h>


class Track;
class JTrack;
class BFile;
class PlugPanel;



class TrackBoost 
{
	public:
	
			 TrackBoost();
	virtual	~TrackBoost(){};
	

	
	
	virtual	Track*		getTrack() { return NULL;}
	virtual	PlugPanel*	getPanel(){return NULL;}
	
	virtual	void		SaveBoosterSettings(BMessage*){};
	virtual	void		SaveTrackSettings(Track*,BMessage*){};
	virtual	void		LoadBoosterSettings(BMessage*){};
	virtual	void		LoadTrackSettings(Track*,BMessage*){};
	
	virtual	status_t	RefReceived(entry_ref,Track*,BMessage *m=NULL){ return B_OK; };
	
			
	virtual	void		Init()   		{};	// only one time, when the boost is created 
	virtual	void		ResetToSong()   {};	// each time a new song is ready
//	virtual	void		Restart() {};	// EACH TIME A song is going to be close.


			Track*		getCurrentTrack();
			
			int16		Id()   { return id;				}
			const char*	Name() { return name.String();  }

	protected:
		BString	name;	// at least protected?
		int16	id;		// Ask Me to assign
};
#endif

