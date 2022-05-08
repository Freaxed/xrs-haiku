/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TracksPanel.h"
#include "GraphicsDef.h"
#include "Xed_Utils.h"
#include "GfxMsg.h"
#include "Track.h"
#include "Song.h"
#include "TrackManager.h"
#include "JTrack.h"
#include "JMessages.h"
#include "TrackManager.h"
#include "MeasureManager.h"
#include "MainWindow.h"

#include <ScrollBar.h>
#include <stdio.h>
#include <Entry.h>
#include <TranslationUtils.h>
#include <InterfaceKit.h>
#include "Song.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"
#include "TrackInfoBox.h"

TracksPanel::TracksPanel(BRect rect): TrackList(rect),ticks(NULL),fTrackInfoBox(NULL)
{
	curSong = NULL;
	SetViewColor(bkColor);
}

void
TracksPanel::ScrollTo(BPoint p)
{
	TrackList::ScrollTo(p);
	if(ticks) ticks->ScrollTo(p.x,0);	
}

void
TracksPanel::FixScrollBar()
{

	if(Window()->Lock())
	{
		BScrollBar* vScroll = ScrollBar(B_VERTICAL);
		BScrollBar* hScroll = ScrollBar(B_HORIZONTAL);
		if(xnv.Count() == 0)
		{
			vScroll->SetRange(0,0);
			hScroll->SetRange(0,0);
		}
		else
		{	
			const BRect bounds = Bounds();
			BSize size(xnv[xnv.Count()-1]->Frame().right, xnv[xnv.Count()-1]->Frame().bottom);
			if (hScroll != NULL) {
			
				float delta = size.Width() - bounds.Width(),
				proportion = bounds.Width() / size.Width();
				if (delta < 0)
					delta = 0;
				hScroll->SetRange(0, delta);
				hScroll->SetSteps(be_plain_font->Size() * 1.33,
				bounds.Width());
				hScroll->SetProportion(proportion);
			}
			
			if (vScroll != NULL) {
				float delta = size.Height() - bounds.Height(),
					proportion = bounds.Height() / size.Height();
				if (delta < 0)
					delta = 0;

				vScroll->SetRange(0, delta);
				vScroll->SetSteps(be_plain_font->Size() * 1.33,
					bounds.Height());
				vScroll->SetProportion(proportion);
			}
		
		}
		Window()->Unlock();
	}

}

void		
TracksPanel::FrameResized(float new_w,float new_h)
{
	BView::FrameResized(new_w, new_h);
	FixScrollBar();
}

status_t
TracksPanel::Init(BView* tk, TrackInfoBox* lTrackInfoBox)
{
	ticks=tk;
	fTrackInfoBox = lTrackInfoBox;
	tm = TrackManager::Get();
	tm->curPanel = this;
	return B_OK;
}

JTrack*
TracksPanel::getJTrackAt(int h)
{
	return xnv[h];
}
int
TracksPanel::getNumberJTrack()
{
	return xnv.Count();
}

Track*
TracksPanel::getCurrentTrack()
{
	return tm->getCurrentTrack() ; //selTrack;
	
}

void
TracksPanel::AttachedToWindow()
{
	TrackList::AttachedToWindow();
	ValuableManager::Get()->RegisterValuableReceiver(VID_PATTERN_CURRENT, this);
}
void
TracksPanel::DetachedFromWindow()
{
	ValuableManager::Get()->UnregisterValuableReceiver(VID_PATTERN_CURRENT, this);
	TrackList::DetachedFromWindow();
}


void
TracksPanel::RemoveTrack(int h)
{
	JTrack *t = xnv[h];
	
	if(Window()->Lock()){	
		xnv.Erase(h);
		LogTrace("Removing Track %d", h);
		TrackList::RemoveTrack((TrackBlock*)t);
		delete t;
		Window()->Unlock();
	}
	FixScrollBar();
}
void
TracksPanel::AddTrack(int h)
{
	ScrollBar(B_HORIZONTAL)->SetValue(0);
	
	if(Window()->Lock())
	{	
		JTrack* t = tm->MakeJTrack(curSong->getTrackAt(h), 
		                         BRect(BUTTONS_X_START,
		                               BUTTONS_Y_START+(float)h*30.0f-40.0f,
		                               WINDOW_XL-18.0f,
		                               BUTTONS_Y_START+JTRACK_LY+(float)h*30.0f-40.0f),
		                               (int16)h);
		xnv.Add(t);
		t->RControl();
		TrackList::AddTrack((TrackBlock*)t);
		
		BMessage *msg=new BMessage(TRACK_SET);
		msg->AddInt16("id", (int16) h);
		t->Init(msg);
		int selPattern=MeasureManager::Get()->GetCurrentPattern();
		t->ResetToTrack(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h),curSong->GetBeatDivision());
		Window()->Unlock();
	}

	
	SelectTrack(h);
	FixScrollBar();
	LogTrace("JTrack %d added!", h);
}
void
TracksPanel::ResetToSong(Song* s)
{
	BView::ScrollTo(0,0);

	curSong = s;
	
	tm->ResetToSong(s);
	

	int max;
	
	//reverse remove
	max=xnv.Count();
	for(int h=max-1;h>=0;h--)
	{
		RemoveTrack(h);
	}
	
	//Add
	max=curSong->getNumberTrack();
	for(int h=0;h<max;h++)
	{
		LogTrace("TracksPanel::Adding new JTrack %d..", h);
		AddTrack(h);		
	}
	
	//Reset
	int selPattern=MeasureManager::Get()->GetCurrentPattern();
	for(int h=0;h<xnv.Count();h++)
	{
		xnv[h]->ResetToTrack(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h), curSong->GetBeatDivision());
	}
	
	if(curSong->getNumberTrack()>0) SelectTrack(0);
	ScrollBar(B_VERTICAL)->SetSteps(1,10);
	ScrollBar(B_HORIZONTAL)->SetSteps(1,10);
	FixScrollBar();
}
void
TracksPanel::MessageReceived(BMessage* message)
{
	switch(message->what)
	{	
		case MSG_VALUABLE_CHANGED:
		{
			int32 value;
			if (ValuableTools::SearchValues(VID_PATTERN_CURRENT, message, &value)){
					resetPattern(value);
			}
		}		
		break;
		
		case TRACK_SET:
			SelectTrack(message->GetInt16("id", 0));
			LogInfo("Right click to be implemented..");
		break;		
		
		case B_SIMPLE_DATA:
		{
			entry_ref ref;		
			for( int i=0; message->FindRef("refs",i, &ref) == B_OK;i++ )
		  		{
		  			//Create a new track from a user dropped file.
		  			MainWindow::Get()->AddTrack(tm->SendRef(ref,0,message));
		  		}
		}
		  	break;
			
		default:
			BView::MessageReceived(message);
		break;
	}
} 

void
TracksPanel::RenameSelectedTrack()
{
	JTrack*	jtr=tm->getCurrentJTrack();
	if(!jtr) return;
	//
	jtr->InvokeRename();
	
}
void
TracksPanel::muteSelectedTrack()
{
	//ATTENZIONE! ->Mute() si comporta non come semra..	
	tm->getCurrentJTrack()->Mute(false,false);
}
void
TracksPanel::soloSelectedTrack()
{
	//ATTENZIONE! ->Mute() si comporta non come semra..	
	JTrack*	jtr=tm->getCurrentJTrack();
	if(!jtr) return;
	for(int u=0;u<curSong->getNumberTrack();u++)
	{
	
		JTrack *c=getJTrackAt(u);
		if(c!=jtr)	c->Mute(false,true);
			else    c->Mute(true,true);
	}
}
void
TracksPanel::resetPattern(int32 selPattern)
{
	if (!curSong)
		return;

	for(int h=0;h<curSong->getNumberTrack();h++)		
		getJTrackAt(h)->Refresh(curSong->getTrackAt(h)->getPatternAt(selPattern), curSong->GetBeatDivision());
}

void
TracksPanel::RemoveTrackAt(int id)
{
	JTrack*	tr;
	
	LogTrace("Removing the track.. %d\n",id);
	

	if(id < 0) return;
	
	SelectTrack(-1);
	
	RemoveTrack(id);
	
	if(Window()->Lock())
	{
	
	int selPattern=MeasureManager::Get()->GetCurrentPattern();
	for(int h=id;h<xnv.Count();h++){
	
		tr=getJTrackAt(h);
		BMessage *msg=new BMessage(TRACK_SET);
		msg->AddInt16("id", (int16) h);
		tr->Init(msg);
		tr->ResetToTrack(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h), curSong->GetBeatDivision());
			
		}
	Window()->Unlock();
	}
	if(curSong->getNumberTrack()>0)
	{
		SelectTrack(0);
	}

}

void
TracksPanel::SelectTrack(int id)
{	if(id<0)
		tm->SelectTrack(NULL, NULL);
	else
		tm->SelectTrack(getJTrackAt(id), fTrackInfoBox);
}
void
TracksPanel::RefreshGraphics()
{
	if(Window()->Lock()){
//	Reset(curSong);
	Window()->Unlock();
	}
}	
void
TracksPanel::RefreshSelected()
{
	int selPattern=MeasureManager::Get()->GetCurrentPattern();
	if(Window()->Lock()){
	if(tm->getCurrentJTrack())
	tm->getCurrentJTrack()->ResetToTrack(tm->getCurrentJTrack()->getTrack()->getPatternAt(selPattern),tm->getCurrentJTrack()->getTrack(), curSong->GetBeatDivision());
	Window()->Unlock();
	}
}	
void
TracksPanel::Refresh(JTrack* t)
{
	int selPattern = MeasureManager::Get()->GetCurrentPattern();
	
	if( t == NULL) 
		return;
	
	if(Window()->Lock()){
		t->ResetToTrack(t->getTrack()->getPatternAt(selPattern),t->getTrack(), curSong->GetBeatDivision());
		Window()->Unlock();
	}
}	
