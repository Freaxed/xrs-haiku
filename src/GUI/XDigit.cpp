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

XDigit::XDigit(BRect frame, ValuableID id, BString name, BMessage *message, BMessage *state,
		int32 minValue, int32 maxValue,
		uint32 resizingMode, uint32 flags):
		ADigit(frame, message, state, minValue, maxValue, resizingMode, flags),
		ValuableView(0, name)
{
	
	ValuableManager::Get()->RegisterValuableView(id,(ValuableView*)this);
			
	SetValuableID(id);
	SetDefaultChannel(0);
	
	//This two lines should be 'automatic' as soon as we register.. no?:)
	float value = ValuableManager::Get()->RetriveValue(id, 0);
	ADigit::SetValue((int32)value, value);
}


void XDigit::AttachedToWindow(){
	
	ADigit::AttachedToWindow();
	SetSender((ValuableView*)this); //Maybe here we should send an update?
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

