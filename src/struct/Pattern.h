/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
#include <Message.h>

class Note;

class Pattern
{
	public:
				Pattern(int);
		
		Note*	getNoteAt(int);
		void	setNoteAt(int,bool);				
		int 	getNumberNotes();
		void 	setNumberNotes(int);
		
		void 	LoadSettings(BMessage& msg);
		void 	SaveSettings(BMessage& msg);
	
	private:
		
		Vector<Note*> noteList;
};

#endif 
