/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PositionView.h"

#include "Xed_Utils.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"

#include <Window.h>
#include <Message.h>
#include <stdio.h>

#define XD 9.0f
#define YD 18.0f


PositionView::PositionView(BRect frame,int d): BView(frame,"PositionView",B_FOLLOW_NONE,B_WILL_DRAW)
{
			
	pos[0]=pos[1]=pos[2]=-1;
	pat[0]=pat[1]=pat[2]=-1;
	beat[0]=beat[1]=beat[2]=-1;	
	
	digit=XUtils::GetBitmap(23); //FIX

}


void
PositionView::Draw(BRect b)
{
	int i,j,k;
	
	for(i=0;i<3;i++)
		DrawBitmap(digit,BiRect(pos[i]),TRect(i));
	
	i++;
	DrawBitmap(digit,BiRect(11),TRect(3));
		
	for(j=0;j<3;j++)
		DrawBitmap(digit,BiRect(pat[j]),TRect(j+i));
	j++;
	DrawBitmap(digit,BiRect(11),TRect(7));
	
	for(k=0;k<2;k++)
		DrawBitmap(digit,BiRect(beat[k]),TRect(k+i+j));
	
}

void
PositionView::_reset(int* arr,int pos,int v,int bpos)
{
	if(v==arr[pos] ) return;
	arr[pos]= v;
	DrawBitmap(digit,BiRect(arr[pos]),TRect(bpos));
}
void
PositionView::SetTick(int po,int pa,int be)
{
	if(pa==-1) pa=-112;
	if(po==-1) po=-112;
	if(be==-1) be=-12;
	
	pa++;
	po++;
	be++;
		
	if(Window()->Lock())
	{
		_reset(pos,0,po / 100,0);
		_reset(pos,1,(po % 100) / 10,1);
		_reset(pos,2,(po % 100) % 10,2);
		
		_reset(pat,0,pa / 100,4);
		_reset(pat,1,(pa % 100) / 10,5);
		_reset(pat,2,(pa % 100) % 10,6);
		
		_reset(beat,0,be / 10,8);
		_reset(beat,1,be % 10,9);
		
		Window()->Unlock();
	}
}
void
PositionView::SetPos(int po)
{
	if(po==-1) 
		po=-112;
	po++;
	if(Window()->Lock())
	{
		_reset(pos,0,po / 100,0);
		_reset(pos,1,(po % 100) / 10,1);
		_reset(pos,2,(po % 100) % 10,2);
		Window()->Unlock();
	}
}

void
PositionView::SetPat(int pa)
{
	if(pa==-1) 
		pa=-112;
	pa++;		
	if(Window()->Lock())
	{
		_reset(pat,0,pa / 100,4);
		_reset(pat,1,(pa % 100) / 10,5);
		_reset(pat,2,(pa % 100) % 10,6);
		Window()->Unlock();
	}
}

void
PositionView::SetBeat(int be)
{
	if(be==-1) be=-12;
	
	be++;
		
	if(Window()->Lock())
	{
		_reset(beat,0,be / 10,8);
		_reset(beat,1,be % 10,9);
		
		Window()->Unlock();
	}
}

BRect
PositionView::BiRect(int d)
{
	if(d<0) 
		d=10;
	return BRect((float)d*XD,0,(float)d*XD+XD,YD);
}
BRect
PositionView::TRect(int pos)
{
	return BRect((float)pos*XD,0,(float)pos*XD+XD,YD);
}
void
PositionView::AttachedToWindow()
{
	SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
	
	ValuableManager::Get()->RegisterValuableReceiver(VID_TEMPO_BEAT,     this);
	ValuableManager::Get()->RegisterValuableReceiver(VID_TEMPO_PATTERN,  this);
	ValuableManager::Get()->RegisterValuableReceiver(VID_TEMPO_MEASURE,  this);
}

void		
PositionView::DetachedFromWindow()
{
	ValuableManager::Get()->UnregisterValuableReceiver(VID_TEMPO_BEAT,     this);
	ValuableManager::Get()->UnregisterValuableReceiver(VID_TEMPO_PATTERN,  this);
	ValuableManager::Get()->UnregisterValuableReceiver(VID_TEMPO_MEASURE,  this);
	BView::DetachedFromWindow();
	
}
void
PositionView::MessageReceived(BMessage* msg)
{
	if(msg->what == MSG_VALUABLE_CHANGED)
	{
			int32 value = -1;
			if (ValuableTools::SearchValues(VID_TEMPO_BEAT,  msg, &value)) {
				SetBeat(value);
			} else
			if (ValuableTools::SearchValues(VID_TEMPO_PATTERN,  msg, &value)) {
				SetPat(value);
			} else
			if (ValuableTools::SearchValues(VID_TEMPO_MEASURE, msg, &value)) {
				SetPos(value);
			}
	}
		
	else
		BView::MessageReceived(msg);
}
