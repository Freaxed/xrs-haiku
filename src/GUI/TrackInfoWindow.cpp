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

TrackInfoWindow::TrackInfoWindow():XrsWindow(BRect(640, 230, 640+180, 230+100),T_TRACKINFO_TITLE,B_FLOATING_WINDOW,B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_AVOID_FOCUS|B_AUTO_UPDATE_SIZE_LIMITS)
{
	SetName("trackinfo_");
	BString title(T_TRACKINFO_TITLE);
	SetTitle(title.String());

	// Create MIDI input controls
	en = new BCheckBox("", T_TRACKINFO_MIDIN, new BMessage(ENABLE_MIDI_IN));
	ch = new XDigit(BRect(0, 0, 36, 22), VID_EMPTY, "midiBox_midi_in", new BMessage(SET_MIDI_IN), 1, 16);
	ch->SetTarget(this);
#ifndef XRS_MIDI
	en->SetEnabled(false);
	ch->SetEnabled(false);
#endif

	// Create MIDI input box
	BBox* midiBox = new BBox("midiBox");
	midiBox->SetExplicitMinSize(BSize(160, 40));
	BLayoutBuilder::Group<>(midiBox, B_HORIZONTAL)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(en)
		.AddGlue()
		.Add(ch);

	fPanelsContainer = new BGroupView();

	// Build main layout with minimum size
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_SPACING)
		.Add(fPanelsContainer)
		.Add(midiBox)
		.SetExplicitMinSize(BSize(180, 200));

	// Size to fit preferred size
	BSize size = GetLayout()->PreferredSize();
	if (size.width < 180) size.width = 180;
	if (size.height < 200) size.height = 200;
	ResizeTo(size.width, size.height);

	myTrack = NULL;

	LoadConfig();
	WindowManager::Get()->RegisterMe(this,T_TRACKINFO_TITLE);
}

void
TrackInfoWindow::AddPanel(BView* panel)
{
	fPanelsContainer->GroupLayout()->AddView(panel);
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
