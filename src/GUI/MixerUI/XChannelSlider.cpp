/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "XChannelSlider.h"

#include "ValuableManager.h"
#include "Logger.h"
#include "Utils.h"

XChannelSlider::XChannelSlider(		
							BRect area,
							const char * name,
							ValuableID id,
							int channel,
							BMessage * model,int min,int max,
							orientation o):
									
							AChannelSlider(
									area,
									"XChannelSlider",
									NULL,
									model,min,max,
									o),ValuableView(channel, name){
									

	
	if(!ValuableManager::Get()->RegisterValuableView(id, (ValuableView*)this))
	{
		BString str("can't register XChannelSlider (");
		str << name << ") with id : " << id;
		Log(LOG_WARN,str.String());
	}	
	
	SetValuableID(id);
	SetDefaultChannel(channel);
		
	float value=ValuableManager::Get()->RetriveValue(id,channel);
	
	SetValue((long)value);
	
	fThumb = NULL; //disabled, haiku thumb is nice enought ;D LoadIcon("Slider.png");
							
};
									


void XChannelSlider::AttachedToWindow()
{
	AChannelSlider::AttachedToWindow();
	
	SetSender((ValuableView*)this);
	SetMessage(CopyMessage());
	SetModificationMessage(CopyMessage());
	SetTarget(ValuableManager::Get());
}



void XChannelSlider::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case MSG_VALUABLE_CHANGED:
		{
			float value;
			if(msg->FindFloat("valuable:value",&value)==B_OK){	
				msg->PrintToStream();
				if (value != Value()) {
					SetValue(value);
				}
			}
			
		}
	break;

	default:
		BControl::MessageReceived(msg);
	break;
	}

}

void XChannelSlider::DrawThumb()
{
	if(fThumb)
		DrawBitmap(fThumb,ThumbFrame());
	else
		AChannelSlider::DrawThumb();
}

//--

