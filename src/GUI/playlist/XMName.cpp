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
#include "ValuableManager.h"
#include "CommonValuableID.h"

#include <math.h>
#include <stdio.h>
#include <Window.h>
#include <String.h>
#include <Message.h>
#include "gui_defines.h"


XMName::XMName(BRect r):BView(r,"",B_FOLLOW_TOP_BOTTOM,B_WILL_DRAW) , sequence(NULL)
{
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
	ValuableManager::Get()->RegisterValuableReceiver(VID_PATTERN_CURRENT, this);	
}

void
XMName::DetachedFromWindow()
{
	ValuableManager::Get()->UnregisterValuableReceiver(VID_PATTERN_CURRENT, this);	
	BView::DetachedFromWindow();
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
	if(sequence == NULL) 
		return;
		
	for(int y=0;y<y_count;y++)
		_drawName(y);
}

#include "Utils.h"

void
XMName::_drawName(int y)
{
	if(sequence == NULL) 
		return;

	BRect r(0,y*XBOX,98,y*XBOX+XBOX-1);
	
	if (y==sel)
		SetHighColor(255,227,153);	
	else
		SetHighColor(200,200,220);
	
	
	FillRect(r);
	SetLowColor(HighColor());
		
	SetHighColor(0,0,0);
		
	BString s = sequence->GetMeasureName(y);		
	
	BString label("");
	label << y+1 << ": " << s.String();
	
	BPDrawString(label.String(), this, r, B_ALIGN_LEFT, 3.0f);
	
	SetHighColor(169,172,151);
	StrokeLine(BPoint(0,y*XBOX+XBOX-1),BPoint(100,y*XBOX+XBOX-1));
}
void
XMName::_MessForRect(int ay1)
{
	BString text = sequence->GetMeasureName(ay1);
	BRect n(ConvertToScreen(BRect( Bounds().left,ay1*XBOX,Bounds().right,(ay1+1)*XBOX)));
	BMessage* m=new BMessage('ttty');
	m->AddPoint("from",BPoint(n.left,n.top));
	m->AddPoint("to",BPoint(n.right,n.bottom));
	m->AddInt16("pos",ay1);
	m->AddString("name",text.String());
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
		MeasureManager::Get()->SetCurrentPattern(ay1);
	}

}

void
XMName::MessageReceived(BMessage *m)
{
	if(m->what == MSG_VALUABLE_CHANGED)
	{
		int32 value;
		if (ValuableTools::SearchValues(VID_PATTERN_CURRENT, m, &value)){
				Select(value);
		}
	}
	else
		BView::MessageReceived(m);
}
