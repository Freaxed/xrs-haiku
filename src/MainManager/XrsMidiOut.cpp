/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XrsMidiOut.h"
#include "MidiTools.h"


XrsMidiOut::XrsMidiOut():BMidiLocalProducer("XrsMidiOut")
{
	BMessage msg;
	
	if (GetProperties(&msg) == B_OK)
	{
		PrepareMessage(&msg);
		SetProperties(&msg);
	}
			
}

XrsMidiOut*
XrsMidiOut::Get()
{
	static	XrsMidiOut*	instance = NULL;
	if (instance == NULL) 
			instance = new XrsMidiOut();
	return instance;
}
