/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

// TickView II
//	15-12-03 xmas!

#include "TickView.h"

#include "Xed_Utils.h"
#include <Window.h>
#include <Message.h>
#include <stdio.h>


#include "BasicValuableView.h"
#include "ValuableManager.h"

BBitmap*	b_tick;
const float	xinc  = 23.0;
const float space = 170.0;

TickView::TickView(BRect frame): BView(frame,"Juice",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW)
{
	ValuableManager::Get()->RegisterValuableView("time.position.fulltick",new BasicValuableView(0, "TickView", this));	
	tick=-1;
	num_notes=16;
	b_tick=XUtils::GetBitmap(5); //fix
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
		DrawBitmap(b_tick,BPoint(space + (i*xinc) - 7.0f, 3.0f));
	}
}

void
TickView::SetTick(int c,int,int)
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
	SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

void
TickView::MessageReceived(BMessage* msg)
{
	if(msg->what==MSG_VALUABLE_CHANGED)
	{
		SetTick((int32)msg->FindFloat("valuable:value"),0,0);
	}
	else
		BView::MessageReceived(msg);
}
