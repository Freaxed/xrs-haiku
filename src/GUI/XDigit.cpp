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

XDigit::XDigit(BRect frame, ValuableID id, BString name, BMessage *message, BMessage *state,
		int32 minValue, int32 maxValue,
		uint32 resizingMode, uint32 flags):
		ADigit(frame, ValuableTools::CreateMessageForBController(id), state, minValue, maxValue, resizingMode, flags) , vID(id)
{	
	
}


void XDigit::AttachedToWindow()
{
	ADigit::AttachedToWindow();
	SetTarget(ValuableManager::Get());
	//SetReleaseMessage();
	
	ValuableManager::Get()->RegisterValuableReceiver(vID, (ValuableReceiver*)this);
}

void XDigit::DetachedFromWindow() {
	ValuableManager::Get()->UnregisterValuableReceiver(vID, (ValuableReceiver*)this);
	ADigit::DetachedFromWindow();
}

void XDigit::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case MSG_VALUABLE_CHANGED:
		{
			int32 value;
			if (ValuableTools::SearchValues(vID, msg, &value)){
					SetValue(value, false);
			}
		}
	break;

	default:
		ADigit::MessageReceived(msg);
	break;
	}
}

