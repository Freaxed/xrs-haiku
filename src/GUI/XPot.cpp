/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XPot.h"




XPot::XPot(BRect frame, const char *name, ValuableID id, int32 min, int32 max, 
	 BBitmap *p1,BBitmap *p2)
	: APot(frame, name, NULL, NULL, min, max, p1, p2),
	vID(id)
{

}

XPot::~XPot() {}

void XPot::DetachedFromWindow() {
	ValuableManager::Get()->UnregisterValuableReceiver(vID, this);		
	APot::DetachedFromWindow();
}

void XPot::AttachedToWindow() {	
	APot::AttachedToWindow();
	
	SetMessage(ValuableTools::CreateMessageForBController(vID));
	SetTarget(ValuableManager::Get());

	ValuableManager::Get()->RegisterValuableReceiver(vID, this);	
}

void XPot::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case MSG_VALUABLE_CHANGED:
		{
			int32 value;
			if (ValuableTools::SearchValues(vID, msg, &value)){
					UpdateValue(value);
			}
		}
	break;

	default:
		BControl::MessageReceived(msg);
	break;
	}
}
//--
