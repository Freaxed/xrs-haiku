/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TrackList.h"
#include "TrackBlock.h"

#include <Message.h>
#include <Window.h>
#include <stdio.h>

#define SPACE	2.0f

TrackList::TrackList(BRect r):BView(r,"TrackList", B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FRAME_EVENTS)
{}

void
TrackList::AddTrack(TrackBlock *tb)
{
	float y = SPACE;
	
	if(list.Count() > 0)	
		y += list.Last()->Frame().bottom;
	
	
	AddChild((BView*)tb);
	
	tb->MoveTo(0, y);
	list.Add(tb);
}

void
TrackList::RemoveTrack(TrackBlock* t)
{
	RemoveChild(t);
	int pos = _moveAfter(t, -1*t->Bounds().Height()-10);

	if (pos > -1)
		list.Erase(pos);
	
}

void
TrackList::Expanded(TrackBlock* t, float delta)
{
	_moveAfter(t, delta);
	FrameResized(0,0); //fast and fourious
}

void
TrackList::UnExpanded(TrackBlock* b,float delta)
{
	Expanded(b, -1*delta);
}

int
TrackList::_moveAfter(TrackBlock* t, float delta)
{
	int pos = list.IndexOf(t);
		
	for(int i = pos + 1 ; i < list.Count(); i++)
	{
		list[i]->MoveBy(0, delta);
	}	
	
	return pos;
}


