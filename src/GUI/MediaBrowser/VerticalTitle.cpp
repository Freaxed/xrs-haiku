#include "VerticalTitle.h"
#include <Font.h>

VerticalTitle::VerticalTitle(BRect r,const char *txt,BMessage *info):BBox(r,"W",B_FOLLOW_NONE,B_WILL_DRAW)
{
	BFont tmp(be_fixed_font);
	GetFont(&tmp);
	tmp.SetRotation(90.0);	
	tmp.SetSize(14);
	SetFont(&tmp,B_FONT_ROTATION|B_FONT_FACE|B_FONT_SIZE|B_FONT_FAMILY_AND_STYLE);
	text=txt;
	spec=BPoint(0,0);
	msg=info;
	wait_click=false;
	
}
void
VerticalTitle::MouseUp(BPoint p){
		
		if(msg!=NULL && wait_click) Window()->PostMessage(msg,Window());
		wait_click=false;
}
void
VerticalTitle::AttachedToWindow()
{
	SetViewColor(Parent()->ViewColor());
	SetLowColor(Parent()->LowColor());
	text_color.red=0;
	text_color.green=0;
	text_color.blue=0;
}
void
VerticalTitle::SetColor(rgb_color cc){
	color=cc;
	SetViewColor(color);
	SetLowColor(color);
	Invalidate();
}
void
VerticalTitle::Draw(BRect area)
{
	SetHighColor(text_color);
	if(spec==BPoint(0,0))
	DrawString(text.String(),BPoint(Bounds().Width()-6,Bounds().Height()-4));
		else
	DrawString(text.String(),spec);
}
void
VerticalTitle::SetStringPos(BPoint x){	spec=x; Invalidate();}