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
									o),vID(id){
									

	
		
	fThumb = NULL; //disabled, haiku thumb is nice enought ;D LoadIcon("Slider.png");
							
};
									


void XChannelSlider::AttachedToWindow()
{
	AChannelSlider::AttachedToWindow();
	
	
	//SetMessage(CopyMessage());
	//SetModificationMessage(CopyMessage());
	//SetTarget(ValuableManager::Get());
	
	ValuableManager::Get()->RegisterValuableReceiver(vID, this);
}

void XChannelSlider::DetachedFromWindow() 
{
	ValuableManager::Get()->UnregisterValuableReceiver(vID, this);
	BView::DetachedFromWindow();
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

