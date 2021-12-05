/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PositionView.h"

#include "Xed_Utils.h"
#include "ValuableManager.h"
#include "IntValuable.h"

#include <Window.h>
#include <Message.h>
#include <stdio.h>

#define XD 9
#define YD 18


PositionView::PositionView(BRect frame,int d): BView(frame,"Juice",B_FOLLOW_NONE,B_WILL_DRAW)
{
	ValuableManager::Get()->RegisterValuableView("time.position.fulltick",new BasicValuableView(0, "time.position.fulltick_ch0",this));
	ValuableManager::Get()->RegisterValuableView("time.position.fulltick",new BasicValuableView(1, "time.position.fulltick_ch1",this));
	ValuableManager::Get()->RegisterValuableView("time.position.fulltick",new BasicValuableView(2, "time.position.fulltick_ch2",this));
			
	pos[0]=pos[1]=pos[2]=-1;
	pat[0]=pat[1]=pat[2]=-1;
	beat[0]=beat[1]=beat[2]=-1;	
	
	digit=XUtils::GetBitmap(23); //FIX
	space=0;
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
	return BRect(d*XD,0,d*XD+XD,YD);
}
BRect
PositionView::TRect(int pos)
{
	return BRect(space+pos*XD,0,space+pos*XD+XD,YD);
}
void
PositionView::AttachedToWindow()
{
	SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}
void
PositionView::MessageReceived(BMessage* msg)
{
	if(msg->what==MSG_VALUABLE_CHANGED)
	{
		int channel = msg->FindInt16("valuable:value:id");
		switch(channel)
		{
			case 0: //beat
				SetBeat((int32)msg->FindFloat("valuable:value"));	
				break;
			case 1: //pat
				SetPat((int32)msg->FindFloat("valuable:value"));
				break;
			case 2: //pos
				SetPos((int32)msg->FindFloat("valuable:value"));
				break;
			default:
				break;
		}	
	}
		
	else
		BView::MessageReceived(msg);
}
