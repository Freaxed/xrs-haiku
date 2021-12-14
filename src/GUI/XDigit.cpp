/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XDigit.h"
#include "ValuableManager.h"

XDigit::~XDigit() {}

XDigit::XDigit(BRect frame, ValuableID id, BString name, BMessage *message,
		int32 minValue, int32 maxValue,
		uint32 resizingMode, uint32 flags):
		ADigit(frame, message, minValue, maxValue, resizingMode, flags) , vID(id)
{	
	SetName(name);
}

XDigit::XDigit(BRect frame, BMessage *message,
		int32 minValue, int32 maxValue,
		uint32 resizingMode,
		uint32 flags):
		ADigit(frame, message, minValue, maxValue, resizingMode, flags) , vID(VID_EMPTY)
{
	SetName("anonymous");
}


void XDigit::AttachedToWindow()
{
	ADigit::AttachedToWindow();
	if (IS_VID_EMPTY(vID) == false) {
		SetMessage(ValuableTools::CreateMessageForBController(vID));
		SetTarget(ValuableManager::Get());
		ValuableManager::Get()->RegisterValuableReceiver(vID, this);
	}
}

void XDigit::DetachedFromWindow() 
{
	if (false == IS_VID_EMPTY(vID)) {
		ValuableManager::Get()->UnregisterValuableReceiver(vID, this);
	}
	ADigit::DetachedFromWindow();
}

void XDigit::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case MSG_VALUABLE_CHANGED:
		{
			int32 value;
			if (ValuableTools::SearchValues(vID, msg, &value)){
					UpdateValue(value, false);
			}
		}
	break;

	default:
		ADigit::MessageReceived(msg);
	break;
	}
}

