/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _XDigit_H
#define _XDigit_H

#include "ADigit.h"
#include "ValuableManager.h"

class XDigit : public ADigit
{
public:
	XDigit(BRect frame, ValuableID id,  BString name, BMessage *message,
		int32 minValue, int32 maxValue,
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW);
		
	XDigit(BRect frame, BMessage *message,
		int32 minValue, int32 maxValue,
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW);

	
	virtual ~XDigit();
	
	
	//events	
	 void AttachedToWindow();
	 void DetachedFromWindow();
	 void MessageReceived(BMessage* msg);
private:
	ValuableID 	vID;
};

#endif 
