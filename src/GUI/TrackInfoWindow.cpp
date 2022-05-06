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
#include "TrackInfoBox.h"
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

	SetTitle(title.String());
	
	fTrackInfoBox = new TrackInfoBox(Bounds());
	AddChild(fTrackInfoBox);
	

	LoadConfig();
	WindowManager::Get()->RegisterMe(this,T_TRACKINFO_TITLE);
}

void
TrackInfoWindow::RegisterPanel(BView* panel)
{
	fTrackInfoBox->RegisterPanel(panel);
}

TrackInfoWindow::~TrackInfoWindow()
{ 
	SaveConfig(); 
	WindowManager::Get()->UnregisterMe(this);
}

void
TrackInfoWindow::SetTrack(Track* tr){ 
	
	fTrackInfoBox->SetTrack(tr);
}

bool
TrackInfoWindow::QuitRequested()
{ 
	WindowManager::Get()->Switch(this); 
	return false;
}
