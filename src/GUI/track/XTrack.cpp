/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "XTrack.h"
#include "JMessages.h"
#include "Xed_Utils.h"

#include <InterfaceKit.h>
#include <stdio.h>
#include <String.h>

#define	XRS_SIMPLE_DATA	'xrsd'
#define	XRS_BANK_SAMPLE	'xbks'

extern void DrawCentredText(const char * TextToCentre, BFont * FontWithWhichToDraw, BRect RectInWhichToCentre, BView * ViewInWhichToDraw, bool MaxHeight);

void	get_color(BBitmap *bmp,int x,int y,rgb_color *cc){

	switch(bmp->ColorSpace())
	{
		case B_RGB32: 
    	case B_RGBA32:
    	{
    		uint32 offset = (y*bmp->BytesPerRow() / 4)+x;
			cc->blue=((uint8* )bmp->Bits())[offset*4];
			cc->green=((uint8 *)bmp->Bits())[offset*4+1];
			cc->red=((uint8 *)bmp->Bits())[offset*4+2];
			cc->alpha=255;
		}
		break;
		
		default:
		break;	
	}

}

XTrack::XTrack(BRect rect,const char *t): BView(rect,"_xtrack",B_FOLLOW_NONE,B_WILL_DRAW)
{
	SetName(t);
	selected=false;	
	pad=XUtils::GetBitmap(27); //fix
}

void
XTrack::AttachedToWindow()
{
	SetViewColor(Parent()->ViewColor());
	SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_OVERLAY);
	SetDrawingMode(B_OP_ALPHA);
	SetFontSize(12);
}
void
XTrack::Draw(BRect r)
{
	if(pad!=NULL)
	{
	  if(selected){
	   
	   		SetHighColor(255,60,60,255);
    		
    		_drawPad();
    		
    		DrawBitmap(pad);
    		
			SetLowColor(HighColor());
			SetHighColor(255,255,205,255);
		}
	   else
	   	{
	   		SetHighColor(rgb_pad);
    		
    		_drawPad();
    		
    		DrawBitmap(pad);
			
			SetLowColor(HighColor());
			SetHighColor(0,0,0,255);
		}
	}
	BFont f;
	GetFont(&f);
	DrawCentredText(name.String(),&f,BRect(12,7,87,22),this,true);
	
	//DrawString(name.String(),15,BPoint(10,17));
}
void
XTrack::_drawPad()
{
	FillEllipse(BRect(0,0,23,23));
	FillEllipse(BRect(76,0,99,23));
	FillRect(BRect(12,0,87,23));
}
void
XTrack::Init(BMessage *m)
{
	msg=m;
	msg->AddInt32("mouse",0);	
}
void 
XTrack::MouseDown(BPoint where)
{
	int32 key;
	uint32 buttons;
	
	BMessage *m=Window()->CurrentMessage();
	m->FindInt32("modifiers",&key);
	GetMouse(&where, &buttons);
	msg->ReplaceInt32("mouse",buttons);
	
	if(key & B_CONTROL_KEY) 
	{
		SendRenameMessage();
		return;
	}
	
	Looper()->PostMessage(msg,target);
			
}

void
XTrack::SendRenameMessage()
{
	BRect n(ConvertToScreen(Bounds()));
	BMessage *m=new BMessage(CHANGE_NAME);
	m->AddPoint("from",BPoint(n.left,n.top));
	m->AddPoint("to",BPoint(n.right,n.bottom));
	Looper()->PostMessage(m,Parent());
}

void
XTrack::Select()
{
	if(Window()->Lock())
	{
		selected=true;
		Invalidate();
		Window()->Unlock();
	}	
}
void
XTrack::Deselect()
{
	if(Window()->Lock())
	{
		selected=false;
		Invalidate();
		Window()->Unlock();
	}
}
bool
XTrack::isSelected()
{
	return selected;
}
void
XTrack::SetName(const char *t)
{
	name.SetTo(t);
	Invalidate();	
}
void
XTrack::MessageReceived(BMessage* message)
{
	//message->PrintToStream();
	switch(message->what){
	
	case 1347638341: //color dropped from kColor or similar.
		rgb_color *newc;
		newc=new rgb_color;
		ssize_t size;
		size=sizeof(newc);
		message->FindData("RGBColor",'RGBC',0,(const void**)&newc,&size); 
        newc->alpha=255;
        SetPadColor(*newc);
        Invalidate();
        delete newc;
		break;
		
	case B_SIMPLE_DATA:		
		message->what=XRS_SIMPLE_DATA;
		
	case XRS_BANK_SAMPLE:
		Window()->PostMessage(message,Parent());
		break;
	default:
		BView::MessageReceived(message);
		break;
	
	}
}
