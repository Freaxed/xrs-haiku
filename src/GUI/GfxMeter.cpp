/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "GfxMeter.h"
#include "Xed_Utils.h"
#include "ValuableManager.h"
#include "Valuable.h"
#include "Log.h"
#include <Message.h>
#include <stdio.h>
#include <math.h>


GfxMeter::GfxMeter(BRect frame, ValuableID id):BView(frame,  id.String(),  B_FOLLOW_NONE,B_WILL_DRAW)
		,vID(id)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	ValuableManager::Get()->RegisterValuable(vID, 0.0f, 0.0f); //TODO move somewhere.!
		
}

GfxMeter::~GfxMeter() {

}

void
GfxMeter::DetachedFromWindow() 
{
	ValuableManager::Get()->UnregisterValuableReceiver(vID, this);
	BView::DetachedFromWindow();
}

void
GfxMeter::AttachedToWindow()
{	
	off = XUtils::GetBitmap(21);
	on = XUtils::GetBitmap(22);
	
	SetViewBitmap(off);
	
	r_bitmap = on->Bounds();
	
	r_up = r_bitmap;
	r_up.OffsetBy(4,4);
	
	r_down = r_bitmap;
	r_down.OffsetBy(4,12);
	
	width = r_up.Width();
	
	ValuableManager::Get()->RegisterValuableReceiver(vID, this);
	
	BView::AttachedToWindow();
		
}
void GfxMeter::Draw(BRect k)
{
	r_up.right = r_up.left + pixel_l * 100.0f;
	r_bitmap.right = r_bitmap.left + pixel_l * 100.0f;
	
	DrawBitmapAsync(on,r_bitmap,r_up);
	
	r_down.right = r_down.left + pixel_r * 100.0f;
	r_bitmap.right = r_bitmap.left + pixel_r * 100.0f;
	
	DrawBitmap(on,r_bitmap,r_down);
		
}
	
	
void GfxMeter::MessageReceived(BMessage *msg){	
	
	switch(msg->what)
	{
		
		case MSG_VALUABLE_CHANGED:
		{
			float value_r, value_l;
			if(ValuableTools::SearchValues(vID, msg, &value_r, &value_l))
			{					
				if(pixel_l != value_l || 
				   pixel_r != value_r )
				{
					pixel_l = value_l;
					pixel_r = value_r;
					Invalidate();
				}
			}			
		}
		break;
		default:
			_inherited::MessageReceived(msg);
			break;
	}
}
