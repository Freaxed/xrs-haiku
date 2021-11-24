/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SampleView.h"
#include <stdio.h>
#include <stdlib.h>



SampleView::SampleView(BRect vframe): BView(vframe,"x",0,B_WILL_DRAW|B_FRAME_EVENTS)
{
	SetViewColor(50,50,50);
	SetLowColor(0,0,0);
	SetHighColor(255,255,255);
	SetFontSize(8);
	
	//data=NULL;
	buffer=NULL;
	rev=false;
	sampleCount=0;
	left_p=right_p=incx=0;
	
		
}
		
void
SampleView::Init(int chan,short *dat,int32 count,bool c)
{
	buffer=dat;
	if(chan!=0) sampleCount=count/chan;
	rev=c;
	incx=(int)(sampleCount/Bounds().Width());
	Invalidate();
	channels=chan;
}


void
SampleView::Draw(BRect b)
{
	
	
	float y;
 	
 	 	
	
	if(buffer==NULL || sampleCount==0){
		//StrokeLine(BPoint(b.left,zero),BPoint(b.right,zero));
		return;
	}
	
		
	SetHighColor(238,235,227);	
	
	for(int i=0;i<Bounds().IntegerWidth();i++)
	{
		if(!rev)
			right = buffer[i*incx*channels];
		else
			right = buffer[(sampleCount-(i*incx))*channels];
		right *= (int)amp;
		y=(float)((zero)+(right/incy));
	 	StrokeLine(BPoint(i-1,right_p),BPoint(i,zero));
     		StrokeLine(BPoint(i,zero),BPoint(i,y));
     		right_p=y;
     		
 	}
 }
 void
 SampleView::AttachedToWindow()
 {
 	zero=(Bounds().Height())/2;	
 	incy=-32767./zero;
 }
