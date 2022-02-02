/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"PianoControl.h"

#include 	"Note.h"
#include 	"Pattern.h"
#include 	"PianoEdit.h"
#include	"locale.h"

#include 	<Message.h>
#include 	<stdio.h>
#include	<Window.h>


#define	NOTE		'note'
#define	SLIDE		'slid'
#define	NOTESIZE	10.
#define	TOTALSIZE	120.


PianoControl::PianoControl(BRect frame,	char*	name):TrackEnd(frame,  name) 
{
		curPattern=NULL;
		 
		SetDrawingMode(B_OP_ALPHA);
		SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_OVERLAY);
		
		BFont tmp(be_fixed_font);
		w_note=new BStringView(BRect(60,80,100,120),"---","---");
		AddChild(w_note);
		w_note->GetFont(&tmp);
		tmp.SetSize(12);
		w_note->SetFont(&tmp);
		
		
		
		pe=new PianoEdit(BRect(109,0,Bounds().right-B_V_SCROLL_BAR_WIDTH,120));
			
		scroll_view=new BScrollView("XRScroolView",pe,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW,false,true,B_NO_BORDER);
		
		ve=scroll_view->ScrollBar(B_VERTICAL);
		ve->SetValue(380);
		ve->SetSteps(10,10);
		AddChild(scroll_view);
		scroll_view->Hide();
			
}

void	PianoControl::Expand(){
	TrackEnd::Expand();
	pe->MakeFocus(false);
	scroll_view->Show();
}

void	PianoControl::UnExpand(){
	TrackEnd::UnExpand();
	pe->MakeFocus(false);
	scroll_view->Hide();
}			
void	PianoControl::Reset(Pattern* p){
		
		curPattern=p;
		pe->Reset(p);
		Invalidate();
		
}	
	
	
	
 void PianoControl::Draw(BRect r){
		if(curPattern==NULL) return;
		
		//if(!IsExpanded()) {
		
					
			for(int i=(int)r.left;i<100;i++){
			
				// giallo! SetHighColor(254,202,0,255-(int)((float)i*2.5));
				SetHighColor(40,123,187,255-(int)((float)i*2.5));
				StrokeLine(BPoint(i,0),BPoint(i,10));
				
		
			}
				SetHighColor(255,255,255);
				DrawString(T_TRACK_PIANO,BPoint(41,10));
			//	return;
			//}
			
			TrackEnd::Draw(r);
			return;
}
void
PianoControl::Refresh()
{
	ve->Invalidate();
}
void
PianoControl::MessageReceived(BMessage* msg){

	switch(msg->what){
	
	case	MOVESCROLL:
		int pos;
		pos=(107-msg->FindInt16("goto"))-5;
		ve->SetValue(pos*10);
	break;
	case	NOTE:
		int note;
		note=msg->FindInt16("note");
		if(note<0) w_note->SetText("---");
		else
		{
			setNote(note);
		}
	case 'noty':
		//msg->PrintToStream();
		//107-pat->getNoteAt(i)->getNote()
		{
			BRect r=pe->Bounds();
			r.left = msg->FindInt16("note") * 23 + 50;
			r.right = r.left+23;
			pe->Invalidate(r);
		}
	break;
	break;
	default:
		TrackEnd::MessageReceived(msg);
	break;
	}

}
void
PianoControl::setNote(int note)
{
	int oct=note / 12;
	int n=note % 12;
	
	switch(n){
	case 0:
		sprintf(infonote,"C-%1d",oct);
	break;
	case 1:
		sprintf(infonote,"C#%1d",oct);
	break;
	case 2:
		sprintf(infonote,"D-%1d",oct);
	break;
	case 3:
		sprintf(infonote,"D#%1d",oct);
	break;
	case 4:
		sprintf(infonote,"E-%1d",oct);
	break;
	case 5:
		sprintf(infonote,"F-%1d",oct);
	break;
	case 6:
		sprintf(infonote,"F#%1d",oct);
	break;
	case 7:
		sprintf(infonote,"G-%1d",oct);
	break;
	case 8:
		sprintf(infonote,"G#%1d",oct);
	break;
	case 9:
		sprintf(infonote,"A-%1d",oct);
	break;
	case 10:
		sprintf(infonote,"A#-%1d",oct);
	break;
	case 11:
		sprintf(infonote,"B-%1d",oct);
	break;
	default:
		sprintf(infonote,"%d",n);
	break;	
	}
	
	w_note->SetText(infonote);			
}
