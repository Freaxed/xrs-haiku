
#include "KickView.h"
#include <stdio.h>
#include <stdlib.h>



KickView::KickView(BRect vframe): BView(vframe,"x",0,B_WILL_DRAW|B_FRAME_EVENTS)
{
	SetViewColor(50,50,50);
	SetLowColor(0,0,0);
	SetHighColor(255,255,255);
	SetFontSize(8);
	
	data=NULL;

	rev=false;
	sampleCount=0;
	left_p=right_p=incx=0;
	
		
}
		
void
KickView::Init(void *dat,int32 count,bool c)
{
	data=dat;
	sampleCount=count;
	rev=c;
	incx=(int)(sampleCount/Bounds().Width());
	Invalidate();
}


void
KickView::Draw(BRect b)
{
	
	
	float y;
 	
 	 	
	
	if(data==NULL || sampleCount==0){
		//StrokeLine(BPoint(b.left,zero),BPoint(b.right,zero));
		return;
	}
	
		
	SetHighColor(238,235,227);	
	
	/*for(int i=0;i<Bounds().IntegerWidth();i++)
	{
		if(!rev)
			memcpy(&right,(char*)data+(i*incx)*2,2);
		else
			memcpy(&right,(char*)data+(sampleCount*2-(i*incx)*2),2);
		//memcpy(&left,(char*)data+(i*incx)*2+2,2);
		right=right*amp;
		y=(float)((zero)+(right/incy));
	 	StrokeLine(BPoint(i-1,right_p),BPoint(i,zero));
     		StrokeLine(BPoint(i,zero),BPoint(i,y));
     		right_p=y;
     		
 	}*/
 }
 void
 KickView::AttachedToWindow()
 {
 	zero=(Bounds().Height())/2;	
 	incy=-32767./zero;
 }