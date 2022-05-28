/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */
#include 	<Message.h>
#include 	<stdio.h>
#include	<Window.h>

#include 	"PianoControl.h"
#include 	"Note.h"
#include 	"Pattern.h"
#include 	"PianoEdit.h"
#include	"locale.h"

PianoControl::PianoControl(BRect frame,	const char*	name):TrackEnd(frame,  name) 
{
	fCurrentPattern = NULL;
		
	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_OVERLAY);
	
	BFont tmp(be_fixed_font);
	w_note = new BStringView(BRect(40, 80, 100, 120),"---","---");
	
	AddChild(w_note);
	tmp.SetSize(12);
	w_note->SetFont(&tmp);
	w_note->ResizeToPreferred();
	
	
	fPianoView = new PianoEdit(BRect(109, 0, Bounds().right - B_V_SCROLL_BAR_WIDTH - 1, 120));
		
	fScrollView = new BScrollView("XRScroolView", fPianoView, B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW, false, true, B_NO_BORDER);
	
	fVerticalScrollBar = fScrollView->ScrollBar(B_VERTICAL);
	fVerticalScrollBar->SetRange(0, 840);
	fVerticalScrollBar->SetValue(420);
	fVerticalScrollBar->SetSteps(NOTESIZE, NOTESIZE);

	AddChild(fScrollView);

	fScrollView->Hide();
			
}

void	
PianoControl::Expand()
{
	TrackEnd::Expand();
	fPianoView->MakeFocus(false);
	fScrollView->Show();
}

void	
PianoControl::UnExpand()
{
	TrackEnd::UnExpand();
	fPianoView->MakeFocus(false);
	fScrollView->Hide();
}			

void 
PianoControl::Reset(Pattern* p)
{
	fCurrentPattern = p;
	fPianoView->Reset(p);
	Invalidate();
}	
	
void 
PianoControl::Draw(BRect r)
{
	if(!fCurrentPattern)
		return;
			
	for(int i=(int)r.left;i<100;i++)
	{
		SetHighColor(40,123,187,255-(int)((float)i*2.5));
		StrokeLine(BPoint(i,0), BPoint(i,10));
	}

	SetHighColor(255,255,255);
	DrawString(T_TRACK_PIANO,BPoint(41,10));

	TrackEnd::Draw(r);
	return;
}

void
PianoControl::Refresh()
{
	fVerticalScrollBar->Invalidate();
}
void
PianoControl::MessageReceived(BMessage* msg){

	switch(msg->what){
	
	case MOVESCROLL:
		fVerticalScrollBar->SetValue(msg->GetInt16("goto", 60) * NOTESIZE);
	break;

	case NOTE:
	{
		int note = msg->FindInt16("note");
		if(note<0) 
		w_note->SetText("---");
		else
		{
			setNote(note);
		}
		//break ??? 
	}
	case 'noty':
		{
			BRect r=fPianoView->Bounds();
			r.left = msg->FindInt16("note") * 23 + 50;
			r.right = r.left+23;
			fPianoView->Invalidate(r);
		}
	break;
	break;
	default:
		TrackEnd::MessageReceived(msg);
	break;
	}

}

const char notesLetter[12] = {'C','C','D','D','E', 'F', 'F', 'G', 'G', 'A', 'A', 'B'};

void
PianoControl::setNote(int note)
{

	int oct = (note - 12) / 12;
	int n   = (note - 12) % 12;

	char sharp  = fPianoView->IsSharp(n) ? '#' : '-';
	sprintf(infonote,"%c%c%1d (%d)",  notesLetter[n], sharp, oct, note);

	w_note->SetText(infonote);			
}
