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
	
	//char	name[100];	//Better a BString!
	BString	name;
	int16	id;		// Ask Me to assign
	
	virtual	Track*		getTrack() { return NULL;}
	virtual	PlugPanel*	getPanel(){return NULL;}
	
	virtual	bool			Load(Track* cur,int t,BFile* file,int32 va,int32 rt) {return true;}
		
	virtual	void			SaveBoosterSettings(BMessage*){};
	virtual	void			SaveTrackSettings(Track*,BMessage*){};
	virtual	void			LoadBoosterSettings(BMessage*){};
	virtual	void			LoadTrackSettings(Track*,BMessage*){};
	
	virtual	status_t	RefReceived(entry_ref,Track*,BMessage *m=NULL){ return B_OK; };
	
	
	//new in xrs 1.4
	
	/*maybe that in a separate file??*/
			int64		ReadFixed(int64*,BFile* file);
			
	virtual	void		Init(){};  //only one time, when the boost is created 
	virtual	void		Reset(){};				//each time a new song is ready
	virtual	void		Restart() {};			//EACH TIME A song is going to be close.


		Track*				getCurrentTrack();
		//TrackManager*		tm;
			
};
#endif

