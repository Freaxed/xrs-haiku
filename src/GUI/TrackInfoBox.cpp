/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "Track.h"
#include "XDigit.h"
#ifdef XRS_MIDI
	#include "XrsMidiIn.h"
	#define	 SET_MIDI_IN		'stmi'
	#define	 ENABLE_MIDI_IN		'enmi'
#endif
#include	"locale.h"


#include "TrackInfoBox.h"

TrackInfoBox::TrackInfoBox(BRect _r) : BBox(_r, "TrackInfoBox", B_FOLLOW_RIGHT, 0, B_NO_BORDER)
{
#ifdef XRS_MIDI	
	BBox *sampler= new BBox(BRect(8,10,172,42) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW, B_FANCY_BORDER);		
	BRect r(sampler->Bounds());
	r.InsetBy(4,4);
	r.right -= 50;
	r.top   += 2;
	sampler->AddChild(fMidiEnabled = new BCheckBox(r,"",T_TRACKINFO_MIDIN,new BMessage(ENABLE_MIDI_IN)));
	sampler->AddChild(fMidiChannel = new XDigit(BRect(120,5,120+36,5+22), VID_EMPTY ,"sampler_midi_in", new BMessage(SET_MIDI_IN), 1,16));
	fMidiChannel->SetTarget(this);
	fMidiEnabled->SetEnabled(false);
	fMidiChannel->SetEnabled(false);
	bot->AddChild(sampler);
#endif
	fTrack = NULL;
}


void
TrackInfoBox::SetTrack(Track* tr)
{ 
	fTrack = tr;
#ifdef XRS_MIDI
	if (Lock())
	{
		fMidiChannel->UpdateValue(tr->GetMidiInCh()+1, true);
		fMidiEnabled->SetValue(tr->IsMidiInEnable());
		Unlock();
	}
#endif
}

void    
TrackInfoBox::RegisterPanel(BView* panel)
{
    AddChild(panel);
}

void
TrackInfoBox::MessageReceived(BMessage* msg)
{
#ifdef XRS_MIDI
	if(msg->what==SET_MIDI_IN){		
		if(fTrack){
			
			XrsMidiIn::Get()->UnregisterCh(fTrack,fTrack->GetMidiInCh());
			XrsMidiIn::Get()->RegisterCh(fTrack,fMidiChannel->GetValue()-1);
			fTrack->SetMidiInCh(fMidiChannel->GetValue()-1);
		}
	}
	else
	if(msg->what==ENABLE_MIDI_IN){
		if(fTrack){
			fTrack->SetMidiInEnable(fMidiEnabled->Value());
			if(fMidiEnabled->Value())
				XrsMidiIn::Get()->RegisterCh(fTrack,fMidiChannel->GetValue()-1);
			else
				XrsMidiIn::Get()->UnregisterCh(fTrack,fMidiChannel->GetValue()-1);
		}
	}
	else
#endif
	BBox::MessageReceived(msg);
}
