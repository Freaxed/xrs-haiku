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
#include "BasicValuableView.h"
#include "ValuableManager.h"

#include <Message.h>
#include <stdio.h>
#include <math.h>
/*
static rgb_color	clipColor={255,255,255};
static rgb_color	backColor={115,134,167};
static rgb_color	rightColor={106,124,154};
static rgb_color	leftColor={106,124,154};
*/

GfxMeter::GfxMeter(BRect frame, ValuableID id):BView(frame,  id.String(),  B_FOLLOW_NONE,B_WILL_DRAW)
		,fId(id)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	
	//fValues = new IntValuable(2);
	ValuableManager::Get()->RegisterValuable(fId, NullValuable);
	
	//we register the two channel notification
	vleft  = NULL ; 
	vright = NULL ;				
}

GfxMeter::~GfxMeter()
{
	ValuableManager::Get()->UnRegisterValuable(fId);
	
	//delete fValues;
	
	if(!vleft || !vright)
		return;
		
	ValuableManager::Get()->UnRegisterValuableView(fId, vleft);
	ValuableManager::Get()->UnRegisterValuableView(fId, vright);
	
	delete vleft;
	delete vright;	
			
}

void
GfxMeter::AttachedToWindow()
{
	//we register the two channel notification
	vleft  = new BasicValuableView(0, BString("GfxMeter_left"), this );
	vright = new BasicValuableView(1, BString("GfxMeter_left"), this );
	
	ValuableManager::Get()->RegisterValuableView(fId, vleft, false);
	ValuableManager::Get()->RegisterValuableView(fId, vright, false);	
	
	off = XUtils::GetBitmap(21);
	on = XUtils::GetBitmap(22);
	
	SetViewBitmap(off);
	
	r_bitmap = on->Bounds();
	
	r_up = r_bitmap;
	r_up.OffsetBy(4,4);
	
	r_down = r_bitmap;
	r_down.OffsetBy(4,12);
	
	width = r_up.Width();
		
}
void GfxMeter::Draw(BRect k)
{
	r_up.right = r_up.left + pixel_l * 100.;
	r_bitmap.right = r_bitmap.left + pixel_l * 100.;
	
	DrawBitmapAsync(on,r_bitmap,r_up);
	
	r_down.right = r_down.left + pixel_r * 100.;
	r_bitmap.right = r_bitmap.left + pixel_r * 100.;
	
	DrawBitmap(on,r_bitmap,r_down);
		
}
	
	
void GfxMeter::MessageReceived(BMessage *message){	
	
	switch(message->what)
	{
		
		case MSG_VALUABLE_CHANGED:
		{
			//message->PrintToStream();
			float value;
			if(message->FindFloat("valuable:value",&value) == B_OK)
			{	
				int16 id = message->FindInt16("valuable:id");
				
				if(id == 0 && pixel_l != value)
				{
					pixel_l = value;
					Invalidate(); // at least half view!
				}
				else
				if (pixel_r != value)
				{
					pixel_r = value;
					Invalidate(); // at least half view!
				}	
			}			
		}
		break;
		default:
			_inherited::MessageReceived(message);
			break;
	}
}
