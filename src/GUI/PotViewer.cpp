/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PotViewer.h"
#include <String.h>

PotViewer*	
PotViewer::Get()
{
	static	PotViewer*	instance = NULL;
	if (instance == NULL) 
			instance = new PotViewer();
	return instance;
}

PotViewer::PotViewer():BWindow(BRect(-100,-100,-50,-50),"",B_BORDERED_WINDOW_LOOK, B_FLOATING_ALL_WINDOW_FEEL,B_NOT_MOVABLE|B_AVOID_FOCUS){

	AddChild(fText=new BStringView(Bounds(),"value","",B_FOLLOW_ALL_SIDES)); 
	fText->SetFontSize(10);
	fText->SetAlignment(B_ALIGN_CENTER);
	fText->SetViewColor(247,255,146,255);
	Show();
	Hide();
}
void	
PotViewer::InitShow(BView* pot, float distance){

	if(Lock()){
	
		if(!IsHidden()) Hide();

		BRect r = pot->ConvertToScreen(pot->Bounds());
		
		MoveTo(r.left,r.bottom + distance);
		
		SetText("");
		SetSizeLimits(r.right-r.left, 1000.0f, Size().Height(), Size().Height());
		Show();
		
		Unlock();
	}
}

void	
PotViewer::InitHide(){
if(Lock()){
if(!IsHidden()) Hide();
Unlock();}
}

void
PotViewer::SetValue(BString value) {
	if(Lock()){
		if(IsHidden()) 
			return;
		SetText(value);
		Unlock();
	}
}
		
void	
PotViewer::SetValue(int32 value){

	if(Lock()){
		if(IsHidden()) 
			return;
	
		BString s;
		s << value;
		SetText(s);
		Unlock();
	}
}

void	
PotViewer::SetText(BString text)
{
	fText->SetText(text.String());
	float h,w;
	fText->GetPreferredSize(&w,&h);
	ResizeTo(w, h);
}
		

//--
