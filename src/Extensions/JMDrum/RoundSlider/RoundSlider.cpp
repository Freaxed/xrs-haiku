#include "RoundSlider.h"
#include <Bitmap.h>
#include <Window.h>
#include <TextControl.h>
#include <Bitmap.h>
#include <Beep.h>
#include <stdio.h>
#include <stdlib.h>

//#include "knob1.h"
#include "knob2.h"
//#include "blueknobs.h"


BBitmap *round_slider_bitmap;
int rsb_initiated=0;

RoundSlider::RoundSlider(BRect frame,BMessage *message,uint32 resizingMode=B_FOLLOW_NONE)
	: BControl(frame,"RoundSlider","label",message,resizingMode,B_WILL_DRAW|B_NAVIGABLE)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	if(rsb_initiated==0)
	{
		rsb_initiated=1;
		BRect rect(1,1,1632,32);
		round_slider_bitmap=new BBitmap(rect,B_RGB32);
		round_slider_bitmap->SetBits(bmp1632x28,1632*28*3,0,B_RGB32);
	}
	value=0;
	ival=0;
}
RoundSlider::RoundSlider(BMessage *archive)
	: BControl(archive)
{
	SetViewColor(B_TRANSPARENT_COLOR);

	if(rsb_initiated == 0)
	{
		rsb_initiated=1;
		BRect rect(1,1,1632,32);
		round_slider_bitmap=new BBitmap(rect,B_RGB32);
		round_slider_bitmap->SetBits(bmp1632x28,1632*28*3,0,B_RGB32);
	}
	value=0;
	ival=0;
}
RoundSlider::~RoundSlider()
{
}
BArchivable *RoundSlider::Instantiate(BMessage *archive) 
{
	if(validate_instantiation(archive,"RoundSlider"))
		return new RoundSlider(archive);
	return NULL;
}
status_t RoundSlider::Archive(BMessage *archive, bool deep) const
{
	BView::Archive(archive, deep);
	archive->AddString("class", "RoundSlider");
}
void RoundSlider::Draw(BRect where)
{
	int nr=int(value*58.0);
	if(nr==58) nr=57;
	DrawBitmap(round_slider_bitmap, BPoint(-nr*32,0));
}
void RoundSlider::MouseDown(BPoint point)
{
	ydown=point.y;
	buttondown=1;

	Draw(Bounds());
}
void RoundSlider::MouseUp(BPoint point)
{
	buttondown=0;
}
void RoundSlider::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
	BPoint p; uint32 buttons;
	GetMouse(&p,&buttons);
	if((!(buttons & B_PRIMARY_MOUSE_BUTTON)) && buttondown)
		buttondown=0;
	if(buttondown)
	{
		//Check if dragging..
		ival+=ydown-point.y;
		if(ival<0) ival=0;
		if(ival>100) ival=100;
		value=float(ival)/100.0;
		//
		if(ydown-point.y!=0)
		{
			Draw(BRect(0,0,28,32));
			Window()->MessageReceived(new BMessage(100));
		}
		ydown=point.y;
		snooze(20000);
		MouseMoved(p, B_INSIDE_VIEW, message);
	}
}
void RoundSlider::SetValue(float val)
{
	if(val<0) val=0;
	if(val>1) val=1;
	ival=val*100;
	value=val;
	Draw(BRect(0,0,28,32));
}
float RoundSlider::GetValue()
{
	return value;
}
