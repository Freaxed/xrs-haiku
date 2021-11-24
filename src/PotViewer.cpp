/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PotViewer.h"
#include <String.h>

PotViewer::PotViewer():BWindow(BRect(-100,-100,-50,-50),"",B_BORDERED_WINDOW_LOOK, B_FLOATING_ALL_WINDOW_FEEL,B_NOT_MOVABLE|B_AVOID_FOCUS){

	AddChild(fText=new BStringView(Bounds(),"value","",B_FOLLOW_ALL_SIDES)); 
	fText->SetFontSize(10);
	fText->SetAlignment(B_ALIGN_CENTER);
	fText->SetViewColor(247,255,146,255);
	Show();
	Hide();
}
void	
PotViewer::InitShow(APot* pot,float distance){

	if(Lock()){
	
	if(!IsHidden()) Hide();
	
	BRect r(pot->Frame()); //float w,h;
	
	r=pot->ConvertToScreen(pot->Bounds());
	
	MoveTo(r.left,r.bottom + distance);
	
//	pot->GetPreferredSize(&w,&h);
	
	ResizeTo(r.right-r.left,12);
	
	
	Show();
	Unlock();}
}

void	
PotViewer::InitHide(){
if(Lock()){
if(!IsHidden()) Hide();
Unlock();}
}

		
void	
PotViewer::SetValue(long value){

	if(Lock()){
	if(IsHidden()) return;
	BString s;
	s << value;
	fText->SetText(s.String());
	Unlock();}
}
		

//--
