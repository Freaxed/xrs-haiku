/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

/*
	Version 1.4
	With pan support
*/

#include "VolView.h"
#include "GfxMsg.h"
#include "Note.h"
#include "Xed_Utils.h"
#include "locale.h"
#include "GraphicsDef.h" //xrs local definitions

#include <InterfaceKit.h>
#include <stdio.h>
#include <stdlib.h>


#define NOTESTART	160.
#define	CHANGE_VIEW	'chvi'
#define	CHANGE_PAR	'chpa'

static 	void			_createMenu();
static	BPopUpMenu*		menu=NULL;


VolView::VolView(BRect rect):TrackEnd(rect,"")
{
	curPattern = NULL;
	
	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_PIXEL_ALPHA,B_ALPHA_OVERLAY);
	track=false;
	mode_view=VELOCITY_VIEW;
	
	AddChild(tools=new BPictureButton(BRect(27,13,27+10,13+8),"_parx",XUtils::GetPicture(8),XUtils::GetPicture(9),new BMessage(CHANGE_PAR),B_ONE_STATE_BUTTON));
	
	
	multimode=false;
	maxview=PAN_VIEW;
	
	if(menu==NULL)
	 	_createMenu();
		
}

void
VolView::SupportMultiMode(int maxv)
{
	if(multimode) return;
	
	AddChild(change=new BPictureButton(BRect(41,13,41+10,13+8),"_pars",XUtils::GetPicture(6),XUtils::GetPicture(7),new BMessage(CHANGE_VIEW),B_ONE_STATE_BUTTON));
	change->SetTarget(this);
	maxview=maxv;
	multimode=true;
}

void
VolView::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
	
	case 'noty':
		if(IsExpanded())
		_drawNote(msg->FindInt16("note"));
	break;
	case CHANGE_VIEW:
	
		if(mode_view==VELOCITY_VIEW)
			ModeView(PAN_VIEW);
		else
		if(mode_view==PAN_VIEW)
			{
			 if(maxview==PAN_VIEW)
				 ModeView(VELOCITY_VIEW);
				else
				ModeView(SUSTAIN_VIEW);
			}
			else
		if(mode_view==SUSTAIN_VIEW)
			
				 ModeView(VELOCITY_VIEW);
			
		
	break;
	case CHANGE_PAR:
	{
		menu->SetTargetForItems(this);
		BPoint point;
		point.x=tools->Frame().left+1;
		point.y=tools->Frame().bottom+2;
		point=ConvertToScreen(point);
		menu->Go(point,true,true,true);
	
	}
	break;
	case 'rand':
	 menu->FindMarked()->SetMarked(false);
	  _randPar();
	 Invalidate();
	break;
	
	case 'rese':
	 menu->FindMarked()->SetMarked(false);
	 _resetPar();
	 Invalidate();
	break;
	
	default:
	TrackEnd::MessageReceived(msg);
	break;
	}
}

void
VolView::AttachedToWindow()
{
	if(multimode)
	change->SetTarget(this);
	tools->SetTarget(this);
	back=Parent()->ViewColor();
	TrackEnd::AttachedToWindow();
}

void
VolView::ModeView(int32 m)
{
	mode_view=m;
	Invalidate();
}

void
VolView::Draw(BRect r)
{
	if(curPattern==NULL) return;
	
	for(int i=(int)r.left;i<100;i++){
			
			SetHighColor(40,123,187,255-(int)((float)i*2.5));
			StrokeLine(BPoint(i,0),BPoint(i,10));
	
	}
	
	SetHighColor(255,255,255);
	DrawString(T_TRACK_PARAMS,BPoint(41,10));
	
	if(!IsExpanded()) return;
	
	SetHighColor(0,0,0);
	
	
	
	if(mode_view==VELOCITY_VIEW)
		DrawString(T_TRACK_VELOCITY,BPoint(56,21));
	else
	if(mode_view==PAN_VIEW)
		DrawString(T_TRACK_PAN,BPoint(56,21));
	else
	if(mode_view==SUSTAIN_VIEW)
		DrawString(T_TRACK_SUSTAIN,BPoint(56,21));
	
	
	if(r.right<NOTESTART) return;
	if(r.left<NOTESTART) r.left=NOTESTART;
	
	int	ax1=(int)floor((r.left-NOTESTART)/(BUTTON_LX+BUTTON_X_SPACE));
	int	ax2=(int)ceil((r.right-NOTESTART)/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax2>curPattern->getNumberNotes()) ax2=curPattern->getNumberNotes();
		
	
	for(int h=ax1;h<ax2;h++)
			_drawNote(h);
	
}
void
VolView::_drawNote(int h)
{
//clear the box
	BRect rect;
			SetHighColor(back);
			rect.Set(NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,0,NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,BUTTON_LY);
			FillRect(rect);
			
			
			if(mode_view==VELOCITY_VIEW)
			{
			 float vol=curPattern->getNoteAt(h)->getVolume();
			 SetHighColor(84,140,213);
			 
			 
			 
			 rect.Set(NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,BUTTON_LY*(float)(1.-vol),NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,BUTTON_LY);
			}
			else
			if(mode_view==PAN_VIEW)
			{
			//Pan?	
			 float vol=curPattern->getNoteAt(h)->getPan();
			 SetHighColor(255,255,0);
			 if(vol>0) 
			 	rect.Set(NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,(BUTTON_LY/2),NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,(BUTTON_LY/2)+(BUTTON_LY/2)*vol);
			 else
				rect.Set(NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,(BUTTON_LY/2)+(BUTTON_LY/2)*vol,NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,(BUTTON_LY/2));
		
			}
			else
			if(mode_view==SUSTAIN_VIEW)
			{
			 bool slide=curPattern->getNoteAt(h)->getOct()&8;
			 //SetHighColor(140,84,213);
			 SetHighColor(255,255,255);
			 if(slide) 
			 	rect.Set(NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,(BUTTON_LY/4),NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,BUTTON_LY-(BUTTON_LY/4));
			 else
				rect.Set(NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,(BUTTON_LY/2),NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,(BUTTON_LY/2));
		
			}
			
			
			if(!curPattern->getNoteAt(h)->getValue())
			 	 SetHighColor(tint_color(HighColor(), B_DARKEN_1_TINT));
			FillRect(rect);
			
			//Black border
			SetHighColor(0,0,0);
			rect.Set(NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*h+1,0,NOTESTART+(BUTTON_LX+BUTTON_X_SPACE)*(h+1)-BUTTON_X_SPACE-1,BUTTON_LY);
			StrokeRect(rect);
}
void
VolView::MouseMoved(BPoint p, uint32 tra,const BMessage*)
{	
	if(curPattern==NULL ) return;
	int	ax1=(int)floor((p.x-NOTESTART)/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax1>=curPattern->getNumberNotes()) ax1=curPattern->getNumberNotes()-1;
	if(ax1<0) return; 
		
	if (track &&  tra==B_INSIDE_VIEW)
	{
		_setValue(curPattern->getNoteAt(ax1),p.y);	
	
		Invalidate(BRect(NOTESTART+ax1*(BUTTON_LX+BUTTON_X_SPACE),0,NOTESTART+(ax1+1)*(BUTTON_LX+BUTTON_X_SPACE)-1,BUTTON_LY));	
		prev = ax1;
	}
}

void
VolView::MouseUp(BPoint p)
{track=false; prev=-1;}

void
VolView::MouseDown(BPoint p)
{
	if(curPattern==NULL) return;
	int	ax1=(int)floor((p.x-NOTESTART)/(BUTTON_LX+BUTTON_X_SPACE));
	
	if(ax1>= curPattern->getNumberNotes() ) ax1=curPattern->getNumberNotes()-1;
	if(ax1<0) return;
	
	prev = ax1;
	track=true;
	SetMouseEventMask(B_POINTER_EVENTS,	B_LOCK_WINDOW_FOCUS | B_NO_POINTER_HISTORY);
				
	_setValue(curPattern->getNoteAt(ax1),p.y);	
	
	Invalidate(BRect(NOTESTART+ax1*(BUTTON_LX+BUTTON_X_SPACE),0,NOTESTART+(ax1+1)*(BUTTON_LX+BUTTON_X_SPACE)-1,BUTTON_LY));
	
}

void
VolView::_setValue(Note* n,float y)
{
	
	if(mode_view==VELOCITY_VIEW)
		n->setVolume(1.-(y/(float)BUTTON_LY));	
	else
	if(mode_view==PAN_VIEW)
		n->setPan((y/(float)(BUTTON_LY/2))-1.0);
	else
	if(mode_view==SUSTAIN_VIEW)
		n->setOct(n->getOct()^8);
	
}
void
VolView::Reset(Pattern* trk)
{
	curPattern=trk;
	Invalidate();			
}

void
VolView::_resetPar()
{
	if(!curPattern) return;
	
	for(int i=0;i<curPattern->getNumberNotes();i++)
	if(mode_view==VELOCITY_VIEW)
		curPattern->getNoteAt(i)->setVolume(0.8);	
	else
	if(mode_view==PAN_VIEW)
		curPattern->getNoteAt(i)->setPan(0.0);
	else
	if(mode_view==SUSTAIN_VIEW)
		//curPattern->getNoteAt(i)->setOct(curPattern->getNoteAt(i)->getOct()^8);
		curPattern->getNoteAt(i)->setOct(0);
	
}
void
VolView::_randPar()
{
	if(!curPattern) return;
	
	for(int i=0;i<curPattern->getNumberNotes();i++)
	{
		double r = ( (double)rand() / (double)(RAND_MAX+1) );
		
		if(mode_view==VELOCITY_VIEW)
		curPattern->getNoteAt(i)->setVolume(-r);	
		else
		if(mode_view==PAN_VIEW)
		curPattern->getNoteAt(i)->setPan((r*-2)-1.0);
		else
		if(mode_view==SUSTAIN_VIEW)
		if(rand() % 2)
		curPattern->getNoteAt(i)->setOct(0);
		else
		curPattern->getNoteAt(i)->setOct(8);

	}
}
void
_createMenu()
{
    menu=new BPopUpMenu("popup",false);
	menu->AddItem(new BMenuItem(T_GEN_RESET,new BMessage('rese')));
	menu->AddItem(new BMenuItem(T_GEN_RANDOMIZE,new BMessage('rand')));
	srand(10000);
}
