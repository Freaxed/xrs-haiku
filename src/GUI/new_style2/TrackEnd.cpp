/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TrackEnd.h"
#include "TrackBlock.h"
#include "Xed_Utils.h"

#include <Window.h>


TrackEnd::TrackEnd(BRect r, const char* name):BView(r,name,B_FOLLOW_NONE,B_WILL_DRAW) 
{
	AddChild(brez= new BPictureButton(BRect(6,1,6+10,1+8),"_menu",XUtils::GetPicture(0),XUtils::GetPicture(1),new BMessage('rez'),B_TWO_STATE_BUTTON));
	SetViewColor(214,219,239);
	expanded=false;
}

void
TrackEnd::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
	
	case 'rez':
			
					
			if(IsExpanded())
				{
					((TrackBlock*)Parent())->UnExpanded(this,getExpansionSize());
					UnExpand();
					Parent()->FrameResized(0,0); //fast and fourious
				}
				else
				{
					((TrackBlock*)Parent())->Expanded(this,getExpansionSize());
					Expand();
					Parent()->FrameResized(0,0); //fast and fourious
				}	
					
			
			
	break;
	
	default:
		BView::MessageReceived(msg);
	break;
	
	}	
}
void 
TrackEnd::AttachedToWindow()
{
	brez->SetTarget(this);
}

void
TrackEnd::Draw(BRect r)
{
	BView::Draw(r);
}

void
TrackEnd::Expand()
{
	ResizeBy(0,getExpansionSize());
	BRect r(Bounds());
	r.bottom=11;
	Invalidate(r);
	expanded=true;
}
void
TrackEnd::UnExpand()
{
	ResizeBy(0,-1*getExpansionSize());
	BRect r(Bounds());
	r.bottom=11;
	Invalidate(r);
	expanded=false;
}
