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

int tracknote_division = 4 ;	//dirty hack;
int notes_per_measaure = 16;	//dirty hack;

Song::Song()
{
	tempo_bpm = 120;
	setTempo(tempo_bpm);
	file_entry=NULL;
		
	popupdesc=true;
	
	sequence=new Sequence();
	sequence->Reset();
	
	num_notes = NUM_NOTES;
	tracknote_division = 4;
	notes_per_measaure = 16;
	AddMeasure();		// a song MUST have at least one Measure!
}


void
Song::setNumberNotes(int n)
{
	notes_per_measaure=n;
	num_notes=n;
	
	for(int h=0;h<getNumberTrack();h++)
	{
		getTrackAt(h)->setNumberNotes(n);
	}
	//do all!
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



Song::~Song()
{
	for(int h=0;h<getNumberTrack();h++)
		delete 	getTrackAt(h);
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
	fullFile.MakeEmpty();
	for(int h=0;h<getNumberTrack();h++)
	{
	   getTrackAt(h)->moveToPattern(0);
	}		
}

void
Song::setTempo(int bpm)
{
	tempo_bpm = bpm;
}
int
Song::getTempo()
{
	return tempo_bpm;
}


void		
Song::setNoteSize(size_t siz)
{
	note_size = siz;
}

size_t
Song::getNoteSize()
{
	return note_size;
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

