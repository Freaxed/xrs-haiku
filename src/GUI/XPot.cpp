/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XPot.h"




XPot::XPot(BRect frame, const char *name,
	ValuableID id, int32 valuable_channel,
	BMessage *state,
	int32 minValue, int32 maxValue,BBitmap *p1,BBitmap *p2)
	: APot(frame, name, NULL, NULL, minValue,maxValue,p1,p2),
	vID(id)
{

}

XPot::~XPot() {}


void XPot::AttachedToWindow() {	
	APot::AttachedToWindow();
	ValuableManager::Get()->RegisterValuableReceiver(vID,this);	
}

void XPot::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case MSG_VALUABLE_CHANGED: {
	
		float value;
		if(msg->FindFloat(VAL_DATA_KEY, &value)==B_OK){	
				UpdateValue((long)value);
		}
			
	}
	break;

	default:
		BControl::MessageReceived(msg);
	break;
	}
}
//--
