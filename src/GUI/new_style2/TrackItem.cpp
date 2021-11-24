/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TrackItem.h"
#include "XNotesView.h"

#include <stdio.h>
#include <Message.h>
#include <String.h>
#include <Bitmap.h>
#include <TranslationUtils.h>

TrackItem::TrackItem(const char* n,BView* ow):BStringItem(n)
{
	active=true;
	title = BTranslationUtils::GetBitmap("TrackNameOn"); //FIX!
	pad=NULL;
	part=NULL;
	owner=ow;
}
void
TrackItem::Reset(NameItem* p,NameItem* p2=NULL)
{
	pad=p;
	pad2=p2;
	part=new XNotesView(BRect(100,0,200,24),NULL,0);
	((XNotesView*)part)->Init();
	((XNotesView*)part)->setType(0);
	((XNotesView*)part)->Reset(new Pattern(16),false);
	owner->AddChild(part);
	
}
void
TrackItem::ChangeStatus(){
	
	active=!active;
	//InvalidateItem();

}
void
TrackItem::Update(BView *owner, const BFont *font)
{
	SetHeight(24);

}

void
TrackItem::DrawItem(BView* w ,BRect r,bool)
{
	if(pad==NULL || part==NULL) return;
	if(part->Frame().top!=r.top) part->MoveTo(150,r.top);
	if(IsExpanded())
	{
		pad->SetStatus(true,r.top+Height());
		printf("Lexp  %f \n",r.top);
		if(pad2!=NULL)
		pad2->SetStatus(true,r.top+Height());
		}
		else
	
	if(  !IsExpanded())
	{	
		pad->SetStatus(false,r.top+Height());
		printf("Limplode   %f \n",r.top);
		if(pad2!=NULL)
		pad2->SetStatus(false,r.top+Height());
	}	
		
	if(IsSelected()) 
		w->SetHighColor(254,202,0);
	else
		w->SetHighColor(255,255,255);
	w->FillRect(r);
	w->DrawBitmap(title,BPoint(r.left,r.top));	
	w->SetHighColor(0,0,0);
	w->DrawString(Text(),BPoint(r.left+30,r.top+10));	
/*
	
	else w->SetHighColor(255,255,255);
	w->FillRect(r);
	w->SetLowColor(tint_color(w->HighColor(),B_DARKEN_1_TINT));	

	if(active)
		w->SetHighColor(0,255,0);
		else
		w->SetHighColor(255,0,0);
	w->FillRect(BRect(r.left,r.top,r.left+20,r.top+10));
*/
}

	

