/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XMName.h"
#include "GlobalDef.h"
#include "Sequence.h"
#include "JMessages.h"
#include "GfxMsg.h"
#include "MeasureManager.h"
#include "maxcount.h"

#include <math.h>
#include <stdio.h>
#include <Window.h>
#include <String.h>
#include <Message.h>

#define 	XBOX		18

XMName::XMName(BRect r):BView(r,"",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW)
{
	MeasureManager::Get()->RegisterMe(this);
}

void
XMName::Reset(Sequence* s)
{
	sequence=s;
	sel=0;
	y_count=s->getMaxPat();
	Invalidate();
	
}
void
XMName::AttachedToWindow()
{
	SetViewColor(200,200,220);
	SetLowColor(200,200,220);
	SetFontSize(12);
}


void
XMName::Select(int y)
{
	int oldsel=sel;
	sel=y;
	_drawName(sel);	
	_drawName(oldsel);
}

void
XMName::Draw(BRect r)
{
	if(sequence==NULL) return;
		
	for(int y=0;y<y_count;y++)
		_drawName(y);
}

void
XMName::_drawName(int y)
{
	BRect r(0,y*XBOX,98,y*XBOX+XBOX-1);
	
	if(y==sel)
			SetHighColor(255,227,153);	
	else
			SetHighColor(200,200,220);
	
	
	FillRect(r);
	SetLowColor(HighColor());
		
	SetHighColor(0,0,0);
		
	BString* s=sequence->GetMeasureName(y);
		
	if(s!=NULL){
		BString label("");
		label << y+1 << ": " << s->String();
		DrawString(label.String(),BPoint(3,y*XBOX+12));
	} 
	
	SetHighColor(169,172,151);
	StrokeLine(BPoint(0,y*XBOX+XBOX-1),BPoint(100,y*XBOX+XBOX-1));
}
void
XMName::_MessForRect(int ay1)
{
	BString* text=sequence->GetMeasureName(ay1);
	BRect n(ConvertToScreen(BRect( Bounds().left,ay1*XBOX,Bounds().right,(ay1+1)*XBOX)));
	BMessage* m=new BMessage('ttty');
	m->AddPoint("from",BPoint(n.left,n.top));
	m->AddPoint("to",BPoint(n.right,n.bottom));
	m->AddInt16("pos",ay1);
	m->AddString("name",text->String());
	Looper()->PostMessage(m,Parent());
}
void
XMName::MouseDown(BPoint p)
{
	if(sequence==NULL) return;
	
	int	ay1=(int)floor(p.y/XBOX);
	
	int32 key;
	
		
	BMessage *m=Window()->CurrentMessage();
	m->FindInt32("modifiers",&key);
	 
	
	if( ( key & B_CONTROL_KEY)  && m!=NULL)
	{
		_MessForRect(ay1);
		return;
	}
	else
	
	if(ay1!=sel &&  ay1<y_count)
	{
		BMessage* notify=new BMessage(SETPAT);
		notify->AddInt32("be:value",ay1);
		be_app->PostMessage(notify,be_app);
	}

}

void
XMName::MessageReceived(BMessage *m)
{
	if(m->what==SETPAT)
		Select(MeasureManager::Get()->GetCurrentPattern());
	else
		BView::MessageReceived(m);
}
