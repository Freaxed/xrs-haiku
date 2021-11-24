/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XrsMidiIn.h"
#include "Track.h"
#include "Song.h"
#include "Note.h"
#include "MidiTools.h"

#include <stdio.h>
#include <Roster.h>


XrsMidiIn::XrsMidiIn():BMidiLocalConsumer("XrsMidiIn")
{
	BMessage msg;
	
	if (GetProperties(&msg) == B_OK)
	{
		PrepareMessage(&msg);
		SetProperties(&msg);
	}
	
}


void
XrsMidiIn::Reset(Song* s)
{
	for(int i=0;i<16;i++)
		channels[i].MakeEmpty();
	
	for(int i=0;i<s->getNumberTrack();i++)
	{
		Track *trk=s->getTrackAt(i);
		if(trk->IsMidiInEnable())
			RegisterCh(trk,trk->GetMidiInCh());
	}
}

void
XrsMidiIn::RegisterCh(Track* t,int i)
{
	printf("Registered Channel %d for track : %s\n",i,t->getName());
	channels[i].AddItem((void*)t);
	
}
void
XrsMidiIn::UnregisterCh(Track* t,int i)
{
	printf("UnRegistered Channel %d for track : %s\n",i,t->getName());
	channels[i].RemoveItem((void*)t);
}

void
XrsMidiIn::NoteOn(uchar channel, uchar note, uchar velocity,bigtime_t time)
{
	
	int ch=channel;
	
	if(channels[ch].CountItems()==0) return;
	
	Note *n=new Note();
	n->setNote(note);			
	
	XRSVoice 	v;
	Track*		track;
	
	for(int i=0;i<channels[ch].CountItems();i++)
	{
		track=(Track*)channels[ch].ItemAt(i);
		v=track->newVoice(n,0);
		if(v!=NULL) 
			track->voice_list.AddItem((void*)v);
	}			
	/*
	
		Channel go from 0 to 15;
	
	*/
}
void
XrsMidiIn::NoteOff(uchar channel, uchar note, uchar velocity,bigtime_t time)
{
	int ch=channel;
	if(channels[ch].CountItems()==0) return;
	
	
	Track*		track;
	
	for(int i=0;i<channels[ch].CountItems();i++)
	{
		track=(Track*)channels[ch].ItemAt(i);
		track->stopVoice(note);
	}
}
void
XrsMidiIn::ControlChange(uchar channel, uchar controlNumber, 
							   uchar controlValue, 
							   bigtime_t time)
							   
{
							   
}

XrsMidiIn*
XrsMidiIn::Get()
{
	static	XrsMidiIn*	instance = NULL;
	if (instance == NULL) 
			instance = new XrsMidiIn();
	return instance;
}
