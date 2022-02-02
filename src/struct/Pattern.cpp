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
