/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "ADigit.h"
#include "Xed_Utils.h"
#include "TextControlFloater.h"

#include <stdio.h>
#include <string.h>
#include <WindowScreen.h>
#include <Application.h>
#include <Window.h>
#include <Message.h>
#include <String.h>
#include <stdlib.h>
#include "Cursors.h" //local define
#include "Log.h"

#define	DIGITSIZE	10.
#define	HEIGHT		15.
#define	XPOS		3.
#define YPOS		4.


ADigit::ADigit(BRect frame, BMessage *message,
	int32 minValue, int32 maxValue,
	uint32 resizingMode, uint32 flags)
	: BControl(frame,  "_adigit",  "_", message, B_FOLLOW_NONE,B_WILL_DRAW),
	m_fMinValue(minValue), m_fMaxValue(maxValue),
	track(false)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	
	digit = XUtils::GetBitmap(14); //fix
	
	if(digit == NULL) 
		printf("ADigit: no bitmap!\n");
	
	SetValueNoUpdate(m_fMinValue);
	
	sens=5;
}

void
ADigit::SetValue(int32 n)
{
	if(n<m_fMinValue) 
		n=m_fMinValue;
		
	if(n>m_fMaxValue) 
		n=m_fMaxValue;
	
	sprintf(dix,"%3d",(int)n);

	for(int i=0;i<3;i++)
	{
		dig[i]=dix[i]-47;
		if(dig[i]<0) dig[i]=0;
	}
			
	BControl::SetValue(n);
}

void
ADigit::UpdateValue(int32 n, bool invoke)
{	
	SetValue(n);
	if (invoke)
		Invoke();
}
void
ADigit::Draw(BRect box)
{
	

	DrawBitmapAsync(XUtils::GetBitmap(13));
	
	for(int i=0;i<3;i++)
	
		DrawBitmapAsync(digit,BRect(dig[i]*DIGITSIZE,0,dig[i]*DIGITSIZE+DIGITSIZE-1,HEIGHT-1),BRect(XPOS+i*DIGITSIZE,YPOS,XPOS-1+i*DIGITSIZE+DIGITSIZE,YPOS+HEIGHT-1));
	//Synch();	
}
void
ADigit::MouseDown(BPoint p)
{

	// PRESS CONTROL DURING CLICK?
	int32 key;
		
	BMessage *m=Window()->CurrentMessage();
	m->FindInt32("modifiers",&key);
	
	if(key & B_CONTROL_KEY) {
	
		BRect n(ConvertToScreen(Bounds()));
		m=new BMessage('ttr');
		m->AddPoint("from",BPoint(n.left,n.top));
		m->AddPoint("to",BPoint(n.right,n.bottom));
		Looper()->PostMessage(m,this);
		return;
	}
	//
	
	 track=true;
	 m_ptPrev=p;
	 mouse_start=p;
	 ConvertToScreen(&mouse_start);
	 be_app->HideCursor();
	 SetMouseEventMask(B_POINTER_EVENTS,B_NO_POINTER_HISTORY);
	 //Invalidate();
	
}

void
ADigit::MessageReceived(BMessage* msg)
{
	switch(msg->what){
	
	case 'ttr':
	{
		
		BPoint f,t;
		
		msg->FindPoint("from", &f);
		msg->FindPoint("to", &t);
		
		/*  Disabling KeyBoard for MainWindow */
		Window()->WindowActivated(false);
		
		BString txt;
		txt << GetValue();
		
		new TextControlFloater(BRect(f,t),B_ALIGN_LEFT,be_plain_font,txt.String(),this,new BMessage('nset'),new BMessage('nnet'));

	}
	break;
	
	case 'nnet':	
		Window()->WindowActivated(true);
	break;
	
	case 'nset':
	{
		//qui controllo: se è un numero, se è minore del minimo o maggiore del massimo.
		const char* name;
		msg->FindString("_value", &name);
		int32 num=atoi(name);
		UpdateValue(num, true);
		
		Window()->WindowActivated(true);
	}
	
	break;
	default:
		BControl::MessageReceived(msg);
	break;
	}
}

void
ADigit::MouseUp(BPoint p)
{
	if(track) Invoke();
	track=false;
	
	be_app->ShowCursor();
	
	set_mouse(mouse_start);
	
	Invoke();
	SetMouseEventMask(0);
	
}
void
ADigit::MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg)
{
	if(!track){
		if(code==B_INSIDE_VIEW)
			be_app->SetCursor((const void *)(c_h_resize_cursor));
		else
			be_app->SetCursor(B_HAND_CURSOR);
		return;
	}
	
	if(where.y < m_ptPrev.y-sens && Value()+1 <= m_fMaxValue) 
	{
		 UpdateValue(Value()+1, true);
	     set_mouse(mouse_start);	
	}
	else
	if(where.y > m_ptPrev.y+sens && Value()-1 >= m_fMinValue) 
	{
	     UpdateValue(Value()-1, true);
		 set_mouse(mouse_start);	
	}
}
void 
ADigit::set_mouse(BPoint p)
{
	int32 x=(int32)p.x;
	int32 y=(int32)p.y;
	if(x!=0 && y!=0) //bug??
		set_mouse_position(x,y);	
}
