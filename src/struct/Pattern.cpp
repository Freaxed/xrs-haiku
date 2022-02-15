/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Pattern.h"
#include "Note.h"


Pattern::Pattern (int n)
{
	for(int h=0 ;  h<n ; h++)   
		noteList.Add(new Note());
		
	lastVoice=NULL;
}

Note*
Pattern::getNoteAt(int val)
{
	return noteList[val];
}

int
Pattern::getNumberNotes()
{
	return noteList.Count();
}
void
Pattern::setNumberNotes(int n)
{
	
	if (n < getNumberNotes())
	{
		for(int h=getNumberNotes()-1;h>=n;h--)
		{
			Note* note = noteList[h];
			noteList.Erase(h);
			delete note;	
		}
	}
	else
	if (n > getNumberNotes())
	{
		for(int h=getNumberNotes();h<n;h++)
			noteList.Add(new Note());
	}
	
}

void
Pattern::LoadSettings(BMessage& pattern)
{
	int16 numNotes = pattern.GetInt16("NotesCount", 16);
	
	setNumberNotes(numNotes);
	
	int i=0;
	BMessage note;
	while(pattern.FindMessage("Note", i, &note) == B_OK) {
		if (i < getNumberNotes()) {
			getNoteAt(i)->setValue(note.GetBool("Value", false));
			getNoteAt(i)->SetGain(note.GetFloat("Gain",   0.8f));
			getNoteAt(i)->SetPan(note.GetFloat("Pan", 0.0f));
			getNoteAt(i)->setNote(note.GetInt16("Note", 60));
		}
		i++;
	}
}
void
Pattern::SaveSettings(BMessage& pattern)
{
	pattern.AddInt16("NotesCount", getNumberNotes());
	for(int k=0;k< getNumberNotes();k++)
	{
		BMessage note;
		note.AddBool ("Value", getNoteAt(k)->getValue());
		note.AddFloat("Gain",  getNoteAt(k)->Gain());
		note.AddFloat("Pan",   getNoteAt(k)->Pan());
		note.AddInt16("Note",  getNoteAt(k)->getNote());

		pattern.AddMessage("Note", &note);
	}
}
