/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PianoEdit.h"
#include "Note.h"
#include "Xed_Utils.h"
#include "MainWindow.h"

#include	<stdio.h>

#define	NOTESIZE	10.
#define	TOTALSIZE	120.	// piano Height
#define NOTESTART	50.


PianoEdit::PianoEdit(BRect r): BView(r,"",B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW),pat(NULL)
{
	piano = XUtils::GetBitmap(8); //FIX, remove.
	SetViewColor(B_TRANSPARENT_COLOR);
}

void 
PianoEdit::Draw(BRect r){

			
		/*	if(r.Intersects(BRect(0,0,NOTESTART,120)))	DrawBitmap(piano,BPoint(NOTESTART-24,0));*/
			
	int	pianotop=(int)floor(r.top/(float)TOTALSIZE);
	int	pianobottom=(int)floor(r.bottom/(float)TOTALSIZE);
				
	for(int j=pianotop;j<pianobottom+1;j++) 
		DrawBitmapAsync(piano,BPoint(0,j*TOTALSIZE));
			
	

	if(pat==NULL) return;
			
			
			if(r.right<NOTESTART) return;
			if(r.left<NOTESTART) r.left=NOTESTART;
			
			int	left=(int)floor((r.left-NOTESTART)/(23));
			int	right=(int)ceil((r.right-NOTESTART+1.)/(23));
			int	top=(int)floor((r.top)/(float)NOTESIZE);
			int	bottom=(int)ceil((r.bottom+1.)/(float)NOTESIZE);
			
	
			
			SetHighColor(160,160,160);
			
			if(right>pat->getNumberNotes()) right=pat->getNumberNotes();
			
			for(int j=top;j<bottom;j++)
			
			for(int i=left;i<right;i++){
			
			int n=(j)%12;
			
			if(n==1 || n==3 || n==5 || n==8 || n==10)
			
				SetHighColor(168,199,190);
			else
				SetHighColor(160,192,182);
				
				_drawNote(i,j);
			
			}
}
void
PianoEdit::_drawNote(int i,int j)
{
	if(107-pat->getNoteAt(i)->getNote()==j)
	{
		SetHighColor(239,179,0);
		if(!pat->getNoteAt(i)->getValue())
			SetHighColor(130,130,130);
	}		
		
	FillRect(BRect(i*23+NOTESTART,j*NOTESIZE,i*23+NOTESTART+23,j*NOTESIZE+NOTESIZE));
			
	SetHighColor(153,184,175);
	
	StrokeLine(BPoint(i*23+NOTESTART,j*NOTESIZE),BPoint(i*23+NOTESTART,j*NOTESIZE+NOTESIZE));
	StrokeLine(BPoint(i*23+NOTESTART,j*NOTESIZE),BPoint(i*23+NOTESTART+23,j*NOTESIZE));	
}
void PianoEdit::MouseDown(BPoint point) {
		
	if(!Bounds().Contains(point) || pat==NULL)	return;
	if(point.x<NOTESTART) return;
	
	uint32 buttons;
	GetMouse(&point, &buttons);
	
	int	ax1=(int)floor((point.x-NOTESTART)/(23));
	
	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		
		
		int	ax2=(int)floor(point.y/NOTESIZE);
	
		if(ax1>= pat->getNumberNotes() ) ax1=pat->getNumberNotes()-1;
		if(ax1<0) ax1=0;
	
		int old=pat->getNoteAt(ax1)->getNote();
		
		
	
		Invalidate(BRect(ax1*23+NOTESTART,(107-old)*NOTESIZE,ax1*23+22+NOTESTART,(107-old)*NOTESIZE+NOTESIZE+1));
		pat->getNoteAt(ax1)->setNote(107-ax2);
		Invalidate(BRect(ax1*23+NOTESTART,(ax2)*NOTESIZE,ax1*23+22+NOTESTART,(ax2)*NOTESIZE+NOTESIZE+1));
	}
	 else
		 	{
		 		BMessage* m=new BMessage(MOVESCROLL);
		 		m->AddInt16("goto",pat->getNoteAt(ax1)->getNote());
		 		Window()->PostMessage(m,Parent());	
		 	}
	
	BView::MouseDown(point);
}
void
PianoEdit::ScrollTo(BPoint point)
{
	int	ax2=(int)floor(point.y/NOTESIZE);
	
		
	//post
	BMessage* m=new BMessage(NOTE);
	m->AddInt16("note",107-ax2);
	Window()->PostMessage(m,Parent());	
	BView::ScrollTo(point);
}
void
PianoEdit::MouseMoved(BPoint point, uint32 code,const BMessage *dragDropMsg)
{
	int	ax2=(int)floor(point.y/NOTESIZE);
	
	if(code==B_ENTERED_VIEW )
	{
		MainWindow::Get()->SetWheelTarget(this);
	}
	
	else
	
	if(code==B_EXITED_VIEW )
	{
		ax2=108;
		MainWindow::Get()->SetWheelTarget(NULL);
	}

	
	//post
	BMessage* m=new BMessage(NOTE);
	m->AddInt16("note",107-ax2);
	Window()->PostMessage(m,Parent());	
}

void 
PianoEdit::Reset(Pattern* p)
{

	 pat=p; 
	 Invalidate();

};

