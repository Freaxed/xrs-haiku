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
#include "gui_defines.h"

#define	MARKER_MOVED 'mark'
#define	MARKER_SIZE	 8
#define	GRID_SIZE	 XBOX	


void
XMPoz::UpdateMarkerPosition(int who, int position)
{
	int32 leftStart = (GRID_SIZE * position);
	if (who == 0)
	{
		marker[who].Set( leftStart + 1, 0, leftStart + MARKER_SIZE, 14);
	}
	else
	{
		int32 rightEnd = (GRID_SIZE * position) + GRID_SIZE;
		
		marker[who].Set( rightEnd - MARKER_SIZE - 2, 0, rightEnd - 2, 14);
	}
	fMarkerPosition[who] = position;

	//printf("fMarkerPosition[0,1] = [%d,%d]\n", fMarkerPosition[0], fMarkerPosition[1]);
}


XMPoz::XMPoz(BRect r):BView(r,"XMPoz",B_FOLLOW_TOP|B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW){

	sequence = NULL;
	fTracking = -1;
	UpdateMarkerPosition(0, 0);
	UpdateMarkerPosition(0, 0);
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
	fMarkerPosition[0] = -1;
	fMarkerPosition[1] = -1;

	UpdateMarkerPosition(0, s->loop_points[0]);	
	UpdateMarkerPosition(1, s->loop_points[1]);	

	Invalidate();
	
	factor = (float)(XBOX - 2) / notesPerMeasaure;
}

void
XMPoz::AttachedToWindow()
{
	BView::AttachedToWindow();
	SetLowColor(200, 200, 220);
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
	if ( sequence == NULL ) 
		return;
	
	int	ax1 = (int)floor(r.left/XBOX) - 1;
	int	ax2 = (int)ceil(r.right/XBOX) + 1;

	if (ax2 > X_COUNT) 
		ax2 = X_COUNT;
	
	for(int x=ax1;x<ax2;x++)	
		_drawCell(x);
}

BRect
RectForCell(int pos)
{
	return BRect(pos*XBOX, 0, pos*XBOX+XBOX-1, XBOX-2);
}

void
XMPoz::_drawCell(int x)
{
	if (x<0) return;

	int maxseq = sequence->getMaxSeq();
			
	SetHighColor(200,200,220);
	FillRect(RectForCell(x));
	
		
	if (x == curPat)
	{
		SetHighColor(255,0,0);
		FillRect(BRect(x*XBOX+1,12,x*XBOX+XBOX-1,15));
	}
	if ( x == maxseq && maxseq >= 0)
	{
		SetHighColor(0,0,255);
		FillRect(BRect(x*XBOX+10,12,x*XBOX+XBOX-1,15));
	}
	
	SetHighColor(0,0,0);
	StrokeLine(BPoint(x*XBOX+XBOX-1,0),BPoint(x*XBOX+XBOX-1,XBOX-1));	
	
	static char na[3];
	sprintf(na,"%2d",x+1);
	DrawString(na,BPoint(x*XBOX+3,8));

	if (sequence->loop_enable && ( x >= fMarkerPosition[0] && x <= fMarkerPosition[1] ))
	{
		PushState();
		SetDrawingMode(B_OP_ALPHA);
		SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
		SetHighColor(255, 0, 0, 80);
		FillRect(BRect(x*XBOX , 0 , x*XBOX+XBOX-1, XBOX - 2));
		PopState();
		
		if (x == fMarkerPosition[0])
		{
			SetHighColor(255,255,155);
			FillTriangle(marker[0].LeftTop(),BPoint(marker[0].right,marker[0].Height()/2),BPoint(marker[0].left,marker[0].bottom));
		}
		if (x == fMarkerPosition[1])
		{
			SetHighColor(255,255,155);
			FillTriangle(BPoint(marker[1].left,marker[1].Height()/2),BPoint(marker[1].right,marker[1].top),BPoint(marker[1].right,marker[1].bottom));
		}
	}
}

void
XMPoz::_drawTick(int x)
{	
	float y=(float)x*factor;
	SetHighColor(255,255,255);
	FillRect(BRect(curPat*XBOX+1,12,curPat*XBOX+1+y,15));
}

	
	
void
XMPoz::MouseUp(BPoint p)
{
	fTracking = -1;
}

void
XMPoz::MouseDown(BPoint point)
{
	if(sequence == NULL) return;
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
			fTracking = 0;
		}
		else
		if (marker[1].Contains(point))
		{
			fTracking = 1;
		}
	}
}


void
XMPoz::setPosition(int val,int tick)
{
	int old = curPat;
	curPat = val;
	
	if(Window()->Lock())
	{
		if(old!=val || tick==0)
		{
			 _drawCell(old);
			 _drawCell(curPat);
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
		 _drawCell(curPat);
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
	if(point.x < 0 || fTracking < 0 || !sequence->loop_enable) 
		return;

	int	ax = (int)floor(point.x/GRID_SIZE);
	
	MoveMarker(fTracking, ax);
	
	if( MarkerPosition(0)>MarkerPosition(1))
		MoveMarker((fTracking + 1) % 2, MarkerPosition(fTracking));

}

int
XMPoz::MarkerPosition(int i)
{
	int	ax=((int)floor(marker[i].left/GRID_SIZE));
	return ax;
}

void	
XMPoz::MoveMarker(int num, int newPos)
{
	int	oldPos = sequence->loop_points[num];
	
	sequence->loop_points[num] = newPos;
	
	UpdateMarkerPosition(num, newPos);	
	
	BRect oldRect = RectForCell(oldPos);
	BRect newRect = RectForCell(newPos);
	
	BRect r;
	if (oldPos <= newPos)
		r = BRect(oldRect.LeftTop(), newRect.RightBottom());
	else
		r = BRect(newRect.LeftTop(), oldRect.RightBottom());

	Invalidate(r);
}
