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
	setNumberNotes(n);
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
			getNoteAt(i)->LoadSettings(note);
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
		getNoteAt(k)->SaveSettings(note);
		pattern.AddMessage("Note", &note);
	}
}
