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
#include "ValuableView.h"
#include "ValuableController.h"

class XDigit : public ADigit, public ValuableView, public ValuableController
{
public:
	XDigit(BRect frame, ValuableID id,  BString name, BMessage *message, BMessage *state,
		int32 minValue, int32 maxValue,
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW);

	
	virtual ~XDigit();
	
	//valuableView
	BHandler*	Handler() { return this;};
	
	
	//events	
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage* msg);
};

#endif 
