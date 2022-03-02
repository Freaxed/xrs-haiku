/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Song.h"
#include "Track.h"
#include "Xed_Utils.h"
#include "Sequence.h"
#include "locale.h"

#include <String.h>
#include <stdio.h>
#include <math.h>


Song::Song()
{
	fBeats 			   = 4; //MOVE TO DEFAULTS (ENGINE? CLOCK?)
	fBeatDivision	   = 4; //MOVE TO DEFAULTS (ENGINE? CLOCK?)
	setTempo(120);

	file_entry=NULL;
		
	popupdesc=true;
	
	sequence=new Sequence();
	sequence->Reset();
	


	AddMeasure();		// a song MUST have at least one Measure!
}

Song::~Song()
{
	for(int h=0;h<getNumberTrack();h++)
		delete 	getTrackAt(h);
	
	if (sequence)
		delete sequence;
}


int16 Song::GetBeatDivision() {	return fBeatDivision; }
int16 Song::GetBeats()		  {	return fBeats;		  }

// size_t Song::GetSamplesPerTick() { return fSamplesPerTick; }
// size_t Song::GetSamplesPerBeat() { return fSamplesPerBeat; }


void
Song::setBeatInfo(int16 beats, int16 divison)
{
	fBeatDivision = divison;
	fBeats 		  = beats;

	for(int h=0; h<getNumberTrack(); h++)
	{
		getTrackAt(h)->setNumberNotes(fBeats * fBeatDivision);
	}
}

void
Song::AddMeasure()
{
	// polling su tutte le traccie..
	for(int h=0;h<getNumberTrack();h++)
	{
		getTrackAt(h)->AddMeasure(1,getNumberNotes());
	}

	getSequence()->AddMeasure(T_GEN_MEASURE);
}
int
Song::getIndexOfTrack(Track* tx)
{ 
	return trk.IndexOf((void*)tx);
}

status_t
Song::RemoveMeasure(int num)
{
	if(getSequence()->getMaxPat()-1<1) return B_ERROR;
	
	// polling su tutte le traccie..
	for(int h=0;h<getNumberTrack();h++)
	{
		getTrackAt(h)->RemoveMeasure(num);
	}
	getSequence()->RemoveMeasure(num);
	getSequence()->setMaxPat(getSequence()->getMaxPat()-1);	
	
	return B_OK;
}


void
Song::AddTrack(Track* j)
{
	j->AddMeasure(getNumberMeasure(),getNumberNotes());
	j->moveToPattern(0);
	trk.AddItem((void*)j);
}


void
Song::RemoveTrack(Track* t)
{
	trk.RemoveItem(trk.IndexOf((void*)t));
	delete t;
}

int
Song::MeltTrack(Track* j )
{
	 trk.AddItem((void*)j);
	 return trk.IndexOf((void*)j);	
}

int
Song::getNumberTrack()
{
	return trk.CountItems();
}





Track*
Song::getTrackAt(int val)
{
	return (Track*)trk.ItemAt(val);
}
int
Song::getCurrentMeasure()
{
	return getSequence()->getMaxPat();
}

int
Song::getNumberMeasure()
{
	return getSequence()->getMaxPat();
}

void
Song::setNumberMeasure(int val)
{
	getSequence()->setMaxPat(val);
}
void
Song::Init()
{	
	for(int h=0;h<getNumberTrack();h++)
	{
	   getTrackAt(h)->moveToPattern(0);
	}		
}

void
Song::setTempo(int32 bpm)
{
	fBeatsPerMinutes = bpm;
}

int32
Song::getTempo()
{
	return fBeatsPerMinutes;
}

BEntry*
Song::getEntry()
{
	return file_entry;
}
void
Song::setEntry(BEntry* s)
{
	file_entry = s;
}

