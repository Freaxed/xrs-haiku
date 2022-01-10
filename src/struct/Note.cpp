/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "Note.h"

Note::Note ()
{
	value=false;
	oct=0;
	//oct ^=8; slide on!
	note=60;
}

bool
Note::getValue()
{
	return value;	
}

void 
Note::setValue(bool val)
{
	value=val;		
}

void
Note::setOct(int val)
{
	oct=val;
}
int
Note::getOct()
{
	return oct;	
}
void
Note::Swap()
{
	value=!value;
}
