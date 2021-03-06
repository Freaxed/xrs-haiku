/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"TrackInfoWindow.h"
#include	"Track.h"
#include	"XDigit.h"
#ifdef XRS_MIDI
	#include	"XrsMidiIn.h"
#endif
#include	"locale.h"
#include	"MainWindow.h"
#include	"WindowManager.h"

#define	SET_MIDI_IN		'stmi'
#define	ENABLE_MIDI_IN	'enmi'

TrackInfoWindow*
TrackInfoWindow::Get()
{
 	static	TrackInfoWindow*	instance = NULL;
	if (instance == NULL) 
			instance = new TrackInfoWindow();
	return instance;
}

TrackInfoWindow::TrackInfoWindow():XrsWindow(BRect(640+0,230+0,640+179,230+280),T_TRACKINFO_TITLE,B_FLOATING_WINDOW,B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_AVOID_FOCUS|B_NOT_RESIZABLE)
{	
	SetName("trackinfo_");
	BString title(T_TRACKINFO_TITLE);
	//title << " " << T_TRACKINFO_NOTRACK;
	
	SetTitle(title.String());
	
	//AddChild(new BBox(BRect(0,0,180,281), ""));
	
	BBox*	bot=new BBox(BRect(0,230,180,281),"");
	
	BBox *sampler= new BBox(BRect(8,10,172,42) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW, B_FANCY_BORDER);		
	BRect r(sampler->Bounds());
	r.InsetBy(4,4);
	r.right-=50;
	r.top+=2;
	sampler->AddChild(en = new BCheckBox(r,"",T_TRACKINFO_MIDIN,new BMessage(ENABLE_MIDI_IN)));
	sampler->AddChild(ch = new XDigit(BRect(120,5,120+36,5+22), VID_EMPTY ,"sampler_midi_in", new BMessage(SET_MIDI_IN), 1,16));
	ch->SetTarget(this);
#ifndef XRS_MIDI
	en->SetEnabled(false);
	ch->SetEnabled(false);
#endif
	bot->AddChild(sampler);
	AddChild(bot);
	myTrack=NULL;

	LoadConfig();
	WindowManager::Get()->RegisterMe(this,T_TRACKINFO_TITLE);
}

TrackInfoWindow::~TrackInfoWindow()
{ 
		SaveConfig(); 
		WindowManager::Get()->UnregisterMe(this);
}

void
TrackInfoWindow::SetTrack(Track* tr){ 
	
	myTrack=tr;
#ifdef XRS_MIDI
	if (Lock())
	{
		ch->UpdateValue(tr->GetMidiInCh()+1, true);
		en->SetValue(tr->IsMidiInEnable());
		Unlock();
	}
#endif
}

bool
TrackInfoWindow::QuitRequested()
{ 
	WindowManager::Get()->Switch(this); 
	return false;
}

void
TrackInfoWindow::MessageReceived(BMessage* msg){
#ifdef XRS_MIDI	
	if(msg->what==SET_MIDI_IN){
	
		
		if(myTrack){
			
			XrsMidiIn::Get()->UnregisterCh(myTrack,myTrack->GetMidiInCh());
			XrsMidiIn::Get()->RegisterCh(myTrack,ch->GetValue()-1);
			myTrack->SetMidiInCh(ch->GetValue()-1);
		}
	}
	else
	if(msg->what==ENABLE_MIDI_IN){
		if(myTrack){
			myTrack->SetMidiInEnable(en->Value());
			if(en->Value())
				XrsMidiIn::Get()->RegisterCh(myTrack,ch->GetValue()-1);
			else
				XrsMidiIn::Get()->UnregisterCh(myTrack,ch->GetValue()-1);
		}
	}
	else
#endif
	BWindow::MessageReceived(msg);
}
