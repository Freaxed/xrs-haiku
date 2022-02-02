/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XListView.h"
#include "XListItem.h"
//#include "MainWindow.h"

XListView::XListView(BRect r):BOutlineListView(r,"x_list_view",B_SINGLE_SELECTION_LIST,B_FOLLOW_ALL_SIDES)
{
}

BBitmap*
XListView::createBitmap(const char* txt, BRect *r, BPoint *p, BListItem *s)
{
	float w = r->Width();
	font_height h;
	GetFontHeight(&h);
	float stringWidth = StringWidth(txt) + 30;
	if (stringWidth > w)
		w = stringWidth;

	BBitmap *bit = new BBitmap( BRect(0,0,w - 1,r->Height() -1) ,B_CMAP8, true);
	
	p->x=w/2.0;
	p->y=r->Height()/2.0;
	
	BView *b=new BView(bit->Bounds(),"fake_view",B_FOLLOW_NONE, B_WILL_DRAW);
	BRect rect(b->Bounds());

	bit->AddChild(b);
	bit->Lock();
	s->DrawItem(b,rect,true);
	b->SetHighColor(255,0,0);
	b->StrokeRect(b->Bounds());
	
	b->Flush();
	bit->Unlock();
	return bit;
}
bool
XListView::InitiateDrag(BPoint p ,int32 in ,bool sel)
{
	int32 n = CurrentSelection(0);
	
	if (n<0) 
		return false;
	
	XListItem *s=dynamic_cast<XListItem*>(ItemAt(n));
	
	if(!s)
		return false;

	BMessage *drag_msg = new BMessage(B_SIMPLE_DATA);
	entry_ref ref=s->getRef();
	drag_msg->AddRef("refs", &ref);
	

	BRect r(ItemFrame(n));
	//DragMessage(drag_msg, createBitmap(s->Label().String(),&r,&p,s), 
	//	BPoint(10,10), this);
	
	
	//for haiku (FIX):
	DragMessage(drag_msg, r, this);

	return true;
}
/*
void
XListView::MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg)
{
	if(code==B_ENTERED_VIEW )
		main_window->SetWheelTarget(this);
	
	else
	if(code==B_EXITED_VIEW )
		main_window->SetWheelTarget(NULL);
	
	
	BOutlineListView::MouseMoved(where,code,dragDropMsg);
}
*/
