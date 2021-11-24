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
#include "Logger.h"

XDigit::~XDigit() {}

XDigit::XDigit(BRect frame, ValuableID id, BMessage *message, BMessage *state,
		int32 minValue, int32 maxValue,
		uint32 resizingMode, uint32 flags):
		ADigit(frame, message, state, minValue, maxValue, resizingMode, flags),
		ValuableView(0)
{
	
	if(!ValuableManager::Get()->RegisterValuableView(id,(ValuableView*)this)){
		BString str("can't register XDigit (");
		str << ") with id : " << id;		
		Log(LOG_WARN,str.String());	
	}	
		
	SetValuableID(id);
	SetDefaultChannel(0);
		
	float value=0; //ValuableManager::Get()->RetriveValue(id,valuable_channel);
	SetValue((long)value);
}


void XDigit::AttachedToWindow(){
	
	ADigit::AttachedToWindow();
	SetSender((ValuableView*)this);
}

void XDigit::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case MSG_VALUABLE_CHANGED: {
	
		float value;
		if(msg->FindFloat("valuable:value",&value)==B_OK){	
				//UpdateValue((long)value);
		}
			
	}
	break;

	default:
		ADigit::MessageReceived(msg);
	break;
	}
}

