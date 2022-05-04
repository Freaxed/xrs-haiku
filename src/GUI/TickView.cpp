/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TickView.h"

#include <Window.h>
#include <Message.h>
#include <stdio.h>

#include "Xed_Utils.h"
#include "PositionView.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"


const float	xinc  = 23.0f;
const float space = 169.0f;

TickView::TickView(BRect frame): BBox(frame, "TickView", B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW, B_NO_BORDER)
{
	tick = -1;
	num_notes = 16;
	b_tick = XUtils::GetBitmap(5);
	AddChild(new PositionView( BRect(36, 0, 36 + 54+54, 18), 0));
}

void 
TickView::SetNumberNotes(int d)
{
	num_notes = d;
	Invalidate();
}

void
TickView::Draw(BRect b)
{
	for(float i=0.0f; i<(float)num_notes; i++)
	{
		DrawBitmapAsync(b_tick, BPoint(space + (i*xinc) - 7.0f, 3.0f));
	}
	Sync();
}

void
TickView::SetTick(int c)
{
	if(c == tick) 
		return;
	
	tick = c;
	
	if ( c < 0 ) {
		Invalidate();		
		return;
	}
	
	if(Window()->Lock())
	{
		SetHighColor(255,10,0,255); 
		FillRect(TRect(tick));
		SetHighColor(ViewColor());
				
		if(tick!=0)
			FillRect(TRect(tick-1));
		else
			FillRect(TRect(num_notes-1));		
		
		Window()->Unlock();
	}
}

BRect
TickView::TRect(int d)
{
	return BRect(space + ((float)d * xinc) - 5.0f, 5.0f, space + ((float)d * xinc) + 4.0f, 9.0f);
}

void
TickView::AttachedToWindow()
{
	BBox::AttachedToWindow();
	//SetViewColor(Parent()->ViewColor());
	
	ValuableManager::Get()->RegisterValuableReceiver(VID_TEMPO_BEAT, this);	
	
}

void 		
TickView::DetachedFromWindow()
{
	ValuableManager::Get()->UnregisterValuableReceiver(VID_TEMPO_BEAT, this);	
	BBox::DetachedFromWindow();
}

void
TickView::MessageReceived(BMessage* msg)
{
	int32 tick;
	if(msg->what==MSG_VALUABLE_CHANGED && ValuableTools::SearchValues(VID_TEMPO_BEAT, msg, &tick))
	{
		SetTick(tick);
	}
	else
		BBox::MessageReceived(msg);
}
