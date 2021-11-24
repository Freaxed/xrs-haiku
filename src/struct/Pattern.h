/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _PATTERN_
#define _PATTERN_

#include "GlobalDef.h"
#include "XRSVoice.h"
#include "Vector.h"

class Note;

class Pattern
{
	public:
	
				Pattern(int);
		
		Note*	getNoteAt(int);
		void	setNoteAt(int,bool);				
		int 	getNumberNotes();
		void 	setNumberNotes(int);
		
		XRSVoice	lastVoice;	
	
	private:
		
		Vector<Note*> noteList;
		
};

#endif 
