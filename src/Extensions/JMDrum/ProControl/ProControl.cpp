#include 	"ProControl.h"
#include	"RoundSlider.h"
#include "stdio.h"

extern void DrawCentredText(const char * TextToCentre, BFont * FontWithWhichToDraw, BRect RectInWhichToCentre, BView * ViewInWhichToDraw, bool MaxHeight);


ProControl::ProControl(BPoint w,int32):BControl(BRect(w,w+BPoint(119,39)),"Unknown","",NULL,B_FOLLOW_NONE,B_WILL_DRAW)
{
	r_knob = BRect(1,4,1+31,4+31);
	r_name = BRect(34,0,119,19);
	r_label = BRect(34,20,119,39);
	AddChild(knob=new RoundSlider(r_knob,NULL,B_FOLLOW_NONE));
}
void
ProControl::AttachedToWindow()
{
	SetViewColor(B_TRANSPARENT_COLOR);
	BControl::AttachedToWindow();
}
void
ProControl::SetLabel(const char* txt)
{
	Invalidate();
	BControl::SetLabel(txt);
}
void
ProControl::Draw(BRect r)
{
	SetHighColor(0,0,0);
	StrokeRect(Bounds());
	
	BFont f;
	GetFont(&f);
	
	//Name
	SetHighColor(143,203,214);
	FillRect(r_name);
	
	SetLowColor(143,203,214);
	SetHighColor(0,0,0);
	DrawCentredText(Name(),&f,r_name,this,true);
	
	//Label/Value
	SetHighColor(211,230,255);
	FillRect(r_label);
	SetLowColor(211,230,255);
	SetHighColor(0,0,0);
	DrawCentredText(Label(),&f,r_label,this,true);
	//StrokeRect(r_name);
	//StrokeRect(r_label);
	BControl::Draw(r);
}