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

TickView::TickView(BRect frame,int d): BView(frame,"Juice",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW)
{
	ValuableManager::Get()->RegisterValuableView("time.position.fulltick",new BasicValuableView(0,this));	
	xinc=23;
	tick=-1;
	space=10+160;
	num_notes=16;
	b_tick=XUtils::GetBitmap(5); //fix
}

void 
TickView::SetNumberNotes(int d)
{
	num_notes=d;
	//ResizeTo(23*num_notes+2,Frame().Height());
	Invalidate();
}
void
TickView::Draw(BRect b)
{
	//BBox::Draw(b);	
	for(int i=0;i<num_notes;i++)
	{
		
		//SetPenSize(2);
		//StrokeRect(BRect(space+i*xinc-6,3,space+i*xinc+5,9+5));
		//SetPenSize(1);
		DrawBitmap(b_tick,BPoint(space+i*xinc-7,3));
	}
}

void
TickView::SetTick(int c,int,int)
{
	if(c==tick) return;
	
	tick=c;
	if(c<0){
		Invalidate();
		
		return;
	}
	
	
	
	if(Window()->Lock())
	{
	
		//rgb_color old_c=HighColor();
		
		//if(a==b)
			SetHighColor(255,10,0,255); 
		//   	   else
		//  	SetHighColor(155,10,156,0); 
		   
		
		FillRect(TRect(tick));
		SetHighColor(ViewColor());
		//SetHighColor(75,107,154);
				
		if(tick!=0)
			FillRect(TRect(tick-1));
		else
			FillRect(TRect(num_notes-1));
		
		
		Window()->Unlock();
		//SetHighColor(old_c);
	}
	
	
	
	
}

BRect
TickView::TRect(int d)
{
	return BRect(space+d*xinc-5,5,space+d*xinc+4,9);
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
	/*if(msg->what=='Mtr')
	{
		SetTick(msg->FindInt16("beat"),msg->FindInt16("pat"),msg->FindInt16("curpat"));
	}*/
	if(msg->what==MSG_VALUABLE_CHANGED)
	{
		SetTick((int32)msg->FindFloat("valuable:value"),0,0);
	}
	else
		BView::MessageReceived(msg);
}
