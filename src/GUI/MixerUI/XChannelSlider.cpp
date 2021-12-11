/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "XChannelSlider.h"

XChannelSlider::XChannelSlider(BRect area, const char * name, ValuableID id, orientation o):
				AChannelSlider(area, name, NULL, NULL, 0, 100, o), vID(id)
{
};
									


void XChannelSlider::AttachedToWindow()
{
	AChannelSlider::AttachedToWindow();

	SetModificationMessage(ValuableTools::CreateMessageForBController(vID));
	SetTarget(ValuableManager::Get());
	
	ValuableManager::Get()->RegisterValuableReceiver(vID, this);
}

void XChannelSlider::DetachedFromWindow() 
{
	ValuableManager::Get()->UnregisterValuableReceiver(vID, this);
	AChannelSlider::DetachedFromWindow();
}



void XChannelSlider::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case MSG_VALUABLE_CHANGED:
		{
			int32 value;
			if (ValuableTools::SearchValues(vID, msg, &value)){
					SetValue(value);
			}
		}
		break;
	
		default:
			BControl::MessageReceived(msg);
		break;
	}
}

//--

