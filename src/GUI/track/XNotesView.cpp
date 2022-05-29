/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XNotesView.h"
#include "GraphicsDef.h"
#include "Xed_Utils.h"
#include "GfxMsg.h"
#include "Note.h"

#include <stdio.h>
#include <string.h>
#include <TranslationUtils.h>
#include <InterfaceKit.h>


XNotesView::XNotesView(BRect rect): BControl(rect, "_xnotesview","", NULL, B_FOLLOW_LEFT, B_WILL_DRAW)
{
	picOn   = XUtils::GetBitmap(9);
	picOff  = XUtils::GetBitmap(10);
	picOnB  = XUtils::GetBitmap(11);
	picOffB = XUtils::GetBitmap(12);
		
	curPattern=NULL;
	SetViewColor(B_TRANSPARENT_COLOR);
	SetFont(be_fixed_font);
	SetFontSize(10);
	selected=-1;
	
	notify = new BMessage(NOTIFY_NOTE);
	notify->AddInt16("note",0);

	fBeatDivision = 0;
}
void		
XNotesView::AttachedToWindow()
{
	SetTarget(Parent());
}

XNotesView::~XNotesView()
{
	delete notify;
}

void
XNotesView::Draw(BRect r)
{
	SetDrawingMode(B_OP_ALPHA);
	
	if(curPattern == NULL) 
		return;
	
	BRect rect;
	
	int	ax1=(int)floor(r.left/(BUTTON_LX+BUTTON_X_SPACE));
	int	ax2=(int)ceil(r.right/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax2>curPattern->getNumberNotes()) ax2=curPattern->getNumberNotes();
	
	
	
	for(int h=ax1;h<ax2;h++)
	{	  
		rect.Set((BUTTON_LX+BUTTON_X_SPACE)*h,0,(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE,BUTTON_LY);
	
		if( h % fBeatDivision == 0 )
		{ 			
			if(curPattern->getNoteAt(h)->getValue()) 
					DrawBitmapAsync(picOn,rect); 
				else
					DrawBitmapAsync(picOff,rect);
		}
		else
		{			
			if(curPattern->getNoteAt(h)->getValue()) 
					DrawBitmapAsync(picOnB,rect); 
				else
					DrawBitmapAsync(picOffB,rect); 
		}
	}
}
void
XNotesView::MouseMoved(BPoint p, uint32 tra, const BMessage*)
{	
	if(curPattern==NULL ) return;
	int	ax1=(int)floor(p.x/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax1>=curPattern->getNumberNotes()) ax1=curPattern->getNumberNotes()-1;
	if(ax1<0) ax1=0;
		
	if (IsTracking()  && tra==B_INSIDE_VIEW) {
		
		if(prev != ax1)
		{
			curPattern->getNoteAt(ax1)->setValue(set_state);
			notify->ReplaceInt16("note",ax1);
			Invoke(notify);
		}
		
		Invalidate(BRect(ax1*(BUTTON_LX+BUTTON_X_SPACE),0,(ax1+1)*(BUTTON_LX+BUTTON_X_SPACE)-1,BUTTON_LY));	
		prev = ax1;
	
	}
}

void
XNotesView::MouseUp(BPoint p)
{
	SetTracking(false); 
	prev=-1;
}
	
void
XNotesView::MouseDown(BPoint p)
{
	if(curPattern==NULL) 
		return;
	int	ax1=(int)floor(p.x/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax1>= curPattern->getNumberNotes() ) ax1=curPattern->getNumberNotes()-1;
	if(ax1<0) ax1=0;
	
	prev = ax1;
	SetTracking(true);
	SetMouseEventMask(B_POINTER_EVENTS,
				B_LOCK_WINDOW_FOCUS | B_NO_POINTER_HISTORY);
				
	set_state=!curPattern->getNoteAt(ax1)->getValue();	
	
	
	curPattern->getNoteAt(ax1)->Swap();
	notify->ReplaceInt16("note",ax1);
	Invoke(notify);
		
	Invalidate(BRect(ax1*(BUTTON_LX+BUTTON_X_SPACE),0,(ax1+1)*(BUTTON_LX+BUTTON_X_SPACE)-1,BUTTON_LY));
	
}

void
XNotesView::Reset(Pattern* trk, int16 beatDivision)
{
	curPattern 		= trk;
	fBeatDivision	= beatDivision;
	Invalidate();			
}
