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

#define SPACE	10.

TrackList::TrackList(BRect r):BView(r,"s2", B_FOLLOW_ALL_SIDES,B_WILL_DRAW|B_FRAME_EVENTS)
{}

void
TrackList::AddTrack(TrackBlock *tb)
{
	float y=SPACE;
	
	if(list.Count()!=0)
	
		y=list[list.Count()-1]->Frame().bottom+SPACE;
	
	
	AddChild((BView*)tb);
	
	tb->MoveTo(0,y);
	list.Add(tb);
}
void
TrackList::RemoveTrack(TrackBlock* t)
{
	int32 pos=list.IndexOf(t);
	RemoveChild(t);
	for(int i=pos+1;i<list.Count();i++)
	{
		list[i]->MoveBy(0,-1*t->Bounds().Height()-10);	
	}
	list.Erase(pos);
	
}
void 
TrackList::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		
	default:
		BView::MessageReceived(msg);
	break;
	
	}	
}
void
TrackList::Expanded(TrackBlock* b,float delta)
{
	int32 position = list.IndexOf(b);
	
	if(position<0) 
		return;
		
	for(int32 i=position+1;i<list.Count();i++)
	{
		list[i]->MoveBy(0,delta);
	}	
	
	FrameResized(0,0); //fast and fourious
}
void
TrackList::UnExpanded(TrackBlock* b,float delta)
{
	int32 position = list.IndexOf(b);
	if(position<0) 
		return;
	
	for(int32 i=position+1;i<list.Count();i++)
	{
		list[i]->MoveBy(0,-1*delta);
	}	
	
	FrameResized(0,0); //fast and fourious

}
