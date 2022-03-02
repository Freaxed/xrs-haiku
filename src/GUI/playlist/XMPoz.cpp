/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XMPoz.h"
#include "GlobalDef.h"
#include "Sequence.h"
#include "Message.h"
#include "GfxMsg.h"
#include "maxcount.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"

#include <stdio.h>
#include <Window.h>

#define XBOX		18
#define	MARKER_MOVED 'mark'
#define	MARKER_SIZE	 8
#define	GRID_SIZE	 XBOX	

BRect marker[2];

extern int 	notes_per_measaure;

char na[3];


XMPoz::XMPoz(BRect r):BView(r,"Juice",B_FOLLOW_TOP|B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_DRAW_ON_CHILDREN){

	loop_start=-1;
	loop_end=-1;
	tracking[0]=false;
	tracking[1]=false;
	marker[0].Set(1,0,MARKER_SIZE,14);
	marker[1].Set(MARKER_SIZE+2,0,MARKER_SIZE+1+MARKER_SIZE,14);

	curPat=-1;
}

void
XMPoz::MessageReceived(BMessage* msg)
{
	if(msg->what == MSG_VALUABLE_CHANGED)
	{
		int32 value = -1;
		if (ValuableTools::SearchValues(VID_TEMPO_BEAT,  msg, &value)) {
				setPosition(curPat, value);
		} else
		if (ValuableTools::SearchValues(VID_TEMPO_MEASURE, msg, &value)) {
				setPositionPar(value);
		}
	}

	else
		BView::MessageReceived(msg);
}

void
XMPoz::Reset(Sequence* s, int16 notesPerMeasaure)
{
	sequence=s;
	curPat=-1;
	
	MoveMarker(0,s->loop_points[0],false);
	MoveMarker(1,s->loop_points[1],false);

	Invalidate();
	
	factor = 16.0 / notesPerMeasaure;

}
void
XMPoz::AttachedToWindow()
{
	BView::AttachedToWindow();
	SetLowColor(200,200,220);
	SetViewColor(B_TRANSPARENT_COLOR);
	SetFontSize(7);
	ValuableManager::Get()->RegisterValuableReceiver(VID_TEMPO_BEAT,   this);	
	ValuableManager::Get()->RegisterValuableReceiver(VID_TEMPO_MEASURE, this);	

}
void
XMPoz::DetachedFromWindow()
{
	ValuableManager::Get()->UnregisterValuableReceiver(VID_TEMPO_BEAT,   this);	
	ValuableManager::Get()->UnregisterValuableReceiver(VID_TEMPO_MEASURE,this);	
	BView::AttachedToWindow();
}




void
XMPoz::Draw(BRect r)
{
	if(sequence==NULL) return;
	
		
	int	ax1=(int)floor(r.left/XBOX)-1;
	int	ax2=(int)ceil(r.right/XBOX)+1;

	if (ax2>X_COUNT) ax2=X_COUNT;
	
	
	for(int x=ax1;x<ax2;x++)	_drawCell(x);
	
	// loop points
	//if(!sequence->loop_enable) return;
	
	/*if(r.Intersects(marker[1])){
		SetHighColor(255,255,155);
		FillTriangle(BPoint(marker[1].left,marker[1].Height()/2),BPoint(marker[1].right,marker[1].top),BPoint(marker[1].right,marker[1].bottom));
	}
	if(r.Intersects(marker[0])){
		SetHighColor(255,255,155);
		FillTriangle(marker[0].LeftTop(),BPoint(marker[0].right,marker[0].Height()/2),BPoint(marker[0].left,marker[0].bottom));
	}*/
		
}
void
XMPoz::_drawCell(int x)
{
	int maxseq=sequence->getMaxSeq();
	
	
		
	SetHighColor(200,200,220);
	FillRect(BRect(x*XBOX,0,x*XBOX+XBOX-1,XBOX-2));
	
		
	if(x>=loop_start && x<=loop_end){
				
			SetHighColor(255,255,155);
			FillRect(BRect(x*XBOX+1,0,x*XBOX+XBOX-1,11));
			
	}
	if(x==curPat)
	{
		SetHighColor(255,0,0);
		FillRect(BRect(x*XBOX+1,12,x*XBOX+XBOX-1,15));
	}
	if(x==maxseq && maxseq>=0)
	{
		SetHighColor(0,0,255);
		FillRect(BRect(x*XBOX+10,12,x*XBOX+XBOX-1,15));
	}
	
	SetHighColor(0,0,0);
	StrokeLine(BPoint(x*XBOX+XBOX-1,0),BPoint(x*XBOX+XBOX-1,XBOX-1));	
	
	sprintf(na,"%2d",x+1);
	DrawString(na,BPoint(x*XBOX+3,8));
	
	/*if(marker[1]){
		SetHighColor(255,255,155);
		FillTriangle(BPoint(marker[1].left,marker[1].Height()/2),BPoint(marker[1].right,marker[1].top),BPoint(marker[1].right,marker[1].bottom));
	}
	if(r.Intersects(marker[0])){
		SetHighColor(255,255,155);
		FillTriangle(marker[0].LeftTop(),BPoint(marker[0].right,marker[0].Height()/2),BPoint(marker[0].left,marker[0].bottom));
	}*/
}

void
XMPoz::_drawTick(int x)
{	
	float y=(float)x*factor;
	SetHighColor(255,255,255);
	FillRect(BRect(curPat*XBOX+1,12,curPat*XBOX+1+y,15));
}
void
XMPoz::DrawAfterChildren(BRect r)
{
	if(!sequence->loop_enable) return;
	
	
	if(r.Intersects(marker[1])){
		SetHighColor(255,255,155);
		FillTriangle(BPoint(marker[1].left,marker[1].Height()/2),BPoint(marker[1].right,marker[1].top),BPoint(marker[1].right,marker[1].bottom));
	}
	if(r.Intersects(marker[0])){
		SetHighColor(255,255,155);
		FillTriangle(marker[0].LeftTop(),BPoint(marker[0].right,marker[0].Height()/2),BPoint(marker[0].left,marker[0].bottom));
	}
	/*if(sequence==NULL) return;
	int x=curPat;
	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_OVERLAY);
	SetHighColor(255,0,0,100);
	FillRect(BRect(x*XBOX+1,8,x*XBOX+XBOX,25));
	SetHighColor(255,0,0,100);
	StrokeLine(BPoint(x*XBOX+1+XBOX/2,26),BPoint(x*XBOX+1+XBOX/2,r.bottom));
	//FillRect(BRect(MarkerPosition(0)*XBOX,8,MarkerPosition(1)*XBOX+XBOX-1,90));
	SetHighColor(0,0,0);	*/
}	
	
	
	
void
XMPoz::MouseUp(BPoint p)
{
	if(tracking[0]) 
		sequence->loop_points[0]=MarkerPosition(0);
	if(tracking[1]) 
		sequence->loop_points[1]=MarkerPosition(1);
	
}
void
XMPoz::MouseDown(BPoint point)
{
	if(sequence==NULL) return;
	if(!sequence->loop_enable) return;
	
	int32 buttons;
	Window()->CurrentMessage()->FindInt32("buttons", &buttons);
	if (buttons == B_PRIMARY_MOUSE_BUTTON)
	{
		// move markers with primary (left) mouse button
		SetMouseEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY | B_LOCK_WINDOW_FOCUS);

		// Check if we hit a marker...
		
	
		
		if (marker[0].Contains(point))
		{
			tracking[0]=false;
			BMessage message(MARKER_MOVED);
			message.AddInt16("which", 0);
			DragMessage(&message, BRect(0.0, 0.0, -1.0, -1.0), this);
		}
		else
		if (marker[1].Contains(point))
		{
			tracking[1]=false;
			BMessage message(MARKER_MOVED);
			message.AddInt16("which", 1);
			DragMessage(&message, BRect(0.0, 0.0, -1.0, -1.0), this);
		}
	}
}

void
XMPoz::setMaxs(int seq ,int pat)
{
	int k;
	k=sequence->getMaxSeq();
	Invalidate();
}

void
XMPoz::setPosition(int val,int tick)
{
	int old=curPat;
	curPat=val;
	
	if(Window()->Lock())
	{
		if(old!=val || tick==0)
		
		{
		 _drawCell(old);
		 DrawAfterChildren(BRect(old*XBOX,0,old*XBOX+XBOX-1,XBOX));
		 _drawCell(curPat);
		 DrawAfterChildren(BRect(curPat*XBOX,0,curPat*XBOX+XBOX-1,XBOX));
		}
		_drawTick(tick);
		Window()->Unlock();
	}

}

void
XMPoz::setPositionPar(int val)
{
	int old=curPat;
	curPat=val;
	
	if(Window()->Lock())
	{
		if(old!=val)
		{
		 _drawCell(old);
		 DrawAfterChildren(BRect(old*XBOX,0,old*XBOX+XBOX-1,XBOX));
		 _drawCell(curPat);
		 DrawAfterChildren(BRect(curPat*XBOX,0,curPat*XBOX+XBOX-1,XBOX));
		}
		
		Window()->Unlock();
	}

}

void
XMPoz::MouseMoved(
	BPoint point,
	uint32 transit,
	const BMessage *message)
{

	if (message)
	{
		switch (message->what)
		{
			case MARKER_MOVED:
			{
				if(!sequence->loop_enable) return;
				if(point.x<0) return;
				
				int	ax=((int)floor(point.x/GRID_SIZE));
				int  num=message->FindInt16("which");
					MoveMarker(num,ax);
				if( num==0 && MarkerPosition(0)>MarkerPosition(1))
					MoveMarker(1,MarkerPosition(0));
				if( num==1 && MarkerPosition(0)>MarkerPosition(1))
					MoveMarker(0,MarkerPosition(1));
				
				
			};
			default:break;
		}
		
	}
}

int
XMPoz::MarkerPosition(int i)
{
	int	ax=((int)floor(marker[i].left/GRID_SIZE));
	return ax;
}
void	
XMPoz::MoveMarker(int num,int ax,bool set)
{
	if(sequence && set) {
		if(sequence->loop_points[num]==ax) return;
		else
			sequence->loop_points[num]=ax;
	}
	//printf("Marker %d %d\n",num,ax);
	
	ax *=GRID_SIZE;
	
	BRect temp=marker[num];
	
	if(num==0)			
		marker[num].left=ax+1;
	else
		marker[num].left=ax+MARKER_SIZE+2;
		
	marker[num].right=marker[num].left+MARKER_SIZE-1;
	
	
	if(set){
		Invalidate(temp);
		Invalidate(marker[num]);
	}
}
