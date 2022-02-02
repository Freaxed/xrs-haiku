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


TracksPanel::TracksPanel(BRect rect): TrackList(rect),ticks(NULL)
{
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

	if(Window()->Lock()){
	if(xnv.Count() == 0)
	{
		ScrollBar(B_VERTICAL)->SetRange(0,0);
		ScrollBar(B_HORIZONTAL)->SetRange(0,0);
	}
	else
	{	float old=ScrollBar(B_VERTICAL)->Value();
		
		//Vertical
		float lastH = xnv[xnv.Count()-1]->Frame().bottom+old;
		float ds=lastH-Frame().Height();
		if(ds<0) ds=0;
		//printf("LAST HEIGHT %f %f max %f \n",lastH,old,ds);
		ScrollBar(B_VERTICAL)->SetRange(0,ds);
		ScrollBar(B_VERTICAL)->SetProportion(Frame().Height()/lastH);
		
		//horiz
		old=ScrollBar(B_HORIZONTAL)->Value();
		lastH = xnv[xnv.Count()-1]->Frame().right+old;
		ds=lastH-Frame().Width();
		if(ds<0) ds=0;
		ScrollBar(B_HORIZONTAL)->SetRange(0,ds);
		ScrollBar(B_HORIZONTAL)->SetProportion(Frame().Width()/lastH);
	
	}
	Window()->Unlock();
	}

}


void		
TracksPanel::FrameResized(float new_w,float new_h){
FixScrollBar();
}

status_t
TracksPanel::Init(BView* tk)
{
	ticks=tk;
	tm=TrackManager::Get();
	tm->curPanel=this;
	MeasureManager::Get()->RegisterMe(this);
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
TracksPanel::RemoveTrack(int h)
{
	
	JTrack *t=xnv[h];
	
	if(Window()->Lock()){	
		xnv.Erase(h);
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
	
	if(Window()->Lock()){
	
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
		t->Reset(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h));
		
		Window()->Unlock();
	}
	
	
	SelectTrack(h);
	FixScrollBar();
}
void
TracksPanel::Reset(Song* s)
{
	curSong=s;
	
	tm->Reset(s);
	
	BView::ScrollTo(0,0);
	
	int max;
	
	//Remove
	max=xnv.Count();
	for(int h=max-1;h>=0;h--)
	{
		printf("removing.. %d\n",h);
		RemoveTrack(h);
	}
	
	//Add
	max=curSong->getNumberTrack();
	for(int h=0;h<max;h++)
	{
		AddTrack(h);		
	}
	
	//Reset
	int selPattern=MeasureManager::Get()->GetCurrentPattern();
	for(int h=0;h<xnv.Count();h++)
	{
		xnv[h]->Reset(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h));
	}
	
	if(curSong->getNumberTrack()>0) SelectTrack(0);
	ScrollBar(B_VERTICAL)->SetSteps(1,10);
	ScrollBar(B_HORIZONTAL)->SetSteps(1,10);
	FixScrollBar();
}
void
TracksPanel::MessageReceived(BMessage* message)
{
	int id;
	entry_ref ref;
	
	if(message->what>1000 && message->what<2000)
	{
		//curSong->setModified(true);
		Window()->PostMessage(new BMessage(GENERIC));
	}
		
	switch(message->what)
	{
	
	case SETPAT:
		resetPattern();
	break;
	
	case TRACK_SET:
	
		id=message->FindInt16("id");
		SelectTrack(id);
		//message->PrintToStream();
		if(message->FindInt32("mouse")==2) tm->ShowTrackInfo();
	break;
	
		
	case B_SIMPLE_DATA:
		
		
		int i;
		i=0;
		
		while( message->FindRef("refs",i, &ref) == B_OK )
   		{
   			//Create a new track from a user dropped file.
   			MainWindow::Get()->AddTrack(tm->SendRef(ref,0,message));
			i++;		
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
TracksPanel::resetPattern()
{
	int selPattern=MeasureManager::Get()->GetCurrentPattern();
	
	for(int h=0;h<curSong->getNumberTrack();h++)		
		getJTrackAt(h)->Refresh(curSong->getTrackAt(h)->getPatternAt(selPattern));
}

void
TracksPanel::MouseDown(BPoint)
{
	//if(!Window()->IsActive()) Window()->Activate(true);
}

void
TracksPanel::RemoveTrackAt(int id)
{
	JTrack*	tr;
	
	printf("Rermoving the track.. %d\n",id);
	

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
		tr->Reset(curSong->getTrackAt(h)->getPatternAt(selPattern),curSong->getTrackAt(h));
			
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
		tm->SelectTrack(NULL);
	else
		tm->SelectTrack(getJTrackAt(id));
}
void
TracksPanel::RefreshGraphics()
{
	if(Window()->Lock()){
	Reset(curSong);
	Window()->Unlock();
	}
}	
void
TracksPanel::RefreshSelected()
{
	int selPattern=MeasureManager::Get()->GetCurrentPattern();
	if(Window()->Lock()){
	if(tm->getCurrentJTrack())
	tm->getCurrentJTrack()->Reset(tm->getCurrentJTrack()->getTrack()->getPatternAt(selPattern),tm->getCurrentJTrack()->getTrack());
	Window()->Unlock();
	}
}	
void
TracksPanel::Refresh(JTrack* t)
{
	int selPattern=MeasureManager::Get()->GetCurrentPattern();
	if(t==NULL) return;
	if(Window()->Lock()){
	t->Reset(t->getTrack()->getPatternAt(selPattern),t->getTrack());
	Window()->Unlock();
	}
}	
