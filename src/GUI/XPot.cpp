/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XPot.h"

#include "ValuableManager.h"
#include "Logger.h"


XPot::XPot(BRect frame, const char *name,
	ValuableID id, int32 valuable_channel,
	BMessage *state,
	int32 minValue, int32 maxValue,BBitmap *p1,BBitmap *p2)
	: APot(frame, name, NULL, NULL, minValue,maxValue,p1,p2),
	ValuableView(valuable_channel, BString(name)),
	m_vChannel(valuable_channel)
{
	
	if(!ValuableManager::Get()->RegisterValuableView(id,(ValuableView*)this)){
		BString str("can't register XPot (");
		str << name << ") with id : " << id;		
		Log(LOG_WARN,str.String());	
	}	
	
	
	
	SetValuableID(id);
	SetDefaultChannel(valuable_channel);
		
	float value=ValuableManager::Get()->RetriveValue(id,valuable_channel);
	SetValue((long)value);
}

XPot::~XPot() {}


void XPot::AttachedToWindow(){
	
	APot::AttachedToWindow();
	SetSender((ValuableView*)this);
	SetMessage(CopyMessage());
	SetTarget(ValuableManager::Get());
}

void XPot::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case MSG_VALUABLE_CHANGED: {
	
		float value;
		if(msg->FindFloat("valuable:value",&value)==B_OK){	
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
