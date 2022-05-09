/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XMatrix.h"
#include "GlobalDef.h"
#include "Sequence.h"
#include "GfxMsg.h"
#include "XMPoz.h"
#include "maxcount.h"
#include "MeasureManager.h"
#include "Xed_Utils.h"
#include "MainWindow.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"

#include <math.h>
#include <stdio.h>
#include <Window.h>
#include <Message.h>
#include "gui_defines.h"

rgb_color col_matrix[8];

XMatrix::XMatrix(BRect r, BView* n, XMPoz* k):BView(r,"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW), sequence (NULL)
{
	the_n = n;
	the_s = k;
	curSub = 0;

	//RGB Color Matrix
	col_matrix[0] = rgb_color{ 255,   0,   0, 255 };
	col_matrix[1] = rgb_color{ 255, 255,   0, 255 };
	col_matrix[2] = rgb_color{   0, 255,   0, 255 };
	col_matrix[3] = rgb_color{   0,   0, 255, 255 };
	
	morbido = XUtils::GetBitmap(7);
}

void
XMatrix::ScrollTo(BPoint p)
{	
	BView::ScrollTo(p);
	the_n->ScrollTo(BPoint(0,p.y));
	the_s->ScrollTo(BPoint(p.x,0));
}

void
XMatrix::Reset(Sequence* s)
{
	sequence = s;				
	curPat = 0;
	sel = 0;
	y_count = s->getMaxPat();
	MaxPat = 0;
	Invalidate();
}

void
XMatrix::AttachedToWindow()
{
	SetViewColor(200,200,220);
	SetLowColor(200,200,220);
	SetFontSize(XBOX/2);
	SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_OVERLAY);
	SetDrawingMode(B_OP_ALPHA);
	ValuableManager::Get()->RegisterValuableReceiver(VID_PATTERN_CURRENT, this);	
}

void
XMatrix::DetachedFromWindow()
{
	ValuableManager::Get()->UnregisterValuableReceiver(VID_PATTERN_CURRENT, this);	
	BView::DetachedFromWindow();
}

void
XMatrix::Select(int y)
{
	if(sequence == NULL) 
		return;

	int oldsel = sel;
	sel = y;
	
	if(Window()->Lock())
	{
		for(int x=0;x<X_COUNT;x++)
		{
			_drawBall(x,oldsel);
			_drawBall(x,sel);
		}		
		Window()->Unlock();
	}
}


void
XMatrix::Draw(BRect r)
{
	
	if(sequence == NULL) 
		return;
	
	int	ax1=(int)floor(r.left/XBOX)-1;
	int	ay1=(int)floor(r.top/XBOX)-1;
	int	ax2=(int)ceil(r.right/XBOX)+1;
	int	ay2=(int)ceil(r.bottom/XBOX)+1;
	
	if(ax2 > X_COUNT) 
		ax2 = X_COUNT;
	
	if(ay2 > y_count) 
		ay2 = y_count;
		
	for(int y=ay1;y<ay2;y++) 
	{
		for(int x=ax1;x<ax2;x++)
		{
	 		_drawBall(x,y);
	 	}
	}				
}				

void
XMatrix::_drawBall(int x,int y)
{
	BRect r(x*XBOX,y*XBOX,x*XBOX+XBOX-1,y*XBOX+XBOX-1);
	
	if(y==sel)
		SetHighColor(255,227,153,255);	
	else
		SetHighColor(200,200,220,255);
		
	FillRect(r);
	if(sequence->ItemAt(x,y)!=-1)
	{	
		// matrix Pint! :
		SetHighColor(col_matrix[y % 4]);
		FillEllipse(BRect(x*XBOX,y*XBOX,x*XBOX+XBOX-3,y*XBOX+XBOX-3));
		
		DrawBitmap(morbido,r);
	}
	else
	{
		SetHighColor(0,0,0);
		StrokeLine(BPoint(x*XBOX+XBOX-1,y*XBOX),BPoint(x*XBOX+XBOX-1,y*XBOX+XBOX-1)); // |
		StrokeLine(BPoint(x*XBOX,y*XBOX+XBOX-1),BPoint(x*XBOX+XBOX-1,y*XBOX+XBOX-1)); //  _
	}
	
		
	
}
void
XMatrix::MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg)
{
	MainWindow::Get()->SetWheelTarget(code == B_ENTERED_VIEW ? this : NULL);
	BView::MouseMoved(where,code,dragDropMsg);
}


void
XMatrix::MouseDown(BPoint p)
{
	if(sequence==NULL) return;
	
		
	int	ax1=(int)floor(p.x/XBOX);
	int	ay1=(int)floor(p.y/XBOX);
	
	if(ax1<X_COUNT && ay1<y_count)
	{
		sequence->setItemAt(ax1,ay1);
		
		if(ax1 >= sequence->getMaxSeq()) setMaxs(ax1,ay1);
		if(Window()->Lock())
		{
			_drawBall(ax1,ay1);
			Window()->Unlock();
		}
	}
}
void
XMatrix::setMaxs(int seq,int pat)
{
	the_s->setMaxs(seq,pat);
	Window()->PostMessage(1010);
}



void
XMatrix::MessageReceived(BMessage *m)
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
