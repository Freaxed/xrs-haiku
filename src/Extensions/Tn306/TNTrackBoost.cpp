/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TNTrackBoost.h"
#include "TNTrack.h"
#include "filemaker_msg.h"

#include <Message.h>
#include <File.h>
#include <Application.h>


TNTrackBoost::TNTrackBoost():TrackBoost()
{
	name << "Tn306";
	id=1;
	panel = new TNPanel(this);
}

Track*
TNTrackBoost::getTrack()
{
	return (Track*)(new TNTrack());
}


bool
TNTrackBoost::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	BMessage *msg;
	switch(va)
	{
		case TRACK_SPECIFIC_DATA_START:
			msg = new BMessage();
			msg->Unflatten(file);
			LoadTrackSettings(cur,msg);
			return true;
			break;
		default:
			break;
	}	
		
	return false;
}


