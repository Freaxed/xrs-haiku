/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "Note.h"

#define DEFAULT_NOTE_GAIN  0.8f
#define DEFAULT_NOTE_PAN   0.0f
#define DEFAULT_NOTE_NOTE  60
#define DEFAULT_NOTE_VALUE false

Note::Note ()
{
	value = DEFAULT_NOTE_VALUE;
	oct = 0; //oct ^=8; slide on!	
	note = DEFAULT_NOTE_NOTE;
	SetGain(DEFAULT_NOTE_GAIN);
	SetPan(DEFAULT_NOTE_PAN);
}

bool
Note::getValue()
{
	return value;	
}

void 
Note::setValue(bool val)
{
	value = val;		
}

void
Note::setOct(int val)
{
	oct = val;
}
int
Note::getOct()
{
	return oct;	
}
void
Note::Swap()
{
	value = !value;
}

void 	
Note::LoadSettings(BMessage& note)
{
	setValue(note.GetBool("Value", DEFAULT_NOTE_VALUE));
	SetGain(note.GetFloat("Gain",  DEFAULT_NOTE_GAIN));
	SetPan(note.GetFloat("Pan",    DEFAULT_NOTE_PAN));
	setNote(note.GetInt16("Note",  DEFAULT_NOTE_NOTE));
}

void 	
Note::SaveSettings(BMessage& note)
{
	note.AddBool ("Value", getValue());
	note.AddFloat("Gain",  Gain());
	note.AddFloat("Pan",   Pan());
	note.AddInt16("Note",  getNote());
}