#include 	"JTrack.h"
#include 	"XTrack.h"
#include 	"JMessages.h"
#include 	"GraphicsDef.h"
#include 	"Pattern.h"
#include 	"Track.h"
#include 	"GfxMsg.h"
#include 	"APot.h"
#include 	"TextControlFloater.h"
#include 	"PianoControl.h"
#include 	"VolView.h"
#include 	"XNotesView.h"
#include 	"XDigit.h"
#include	"MixerWindow.h"
#include	"TrackManager.h"
#include 	"ColorsDef.h"
#include	"Xed_Utils.h"
#include	<Entry.h>
#include 	<stdio.h>
#include 	<PictureButton.h>

#define	POT_X		TRACK_LX+TRACK_MENU_LX+70+SBUTTON_W+10
#define	POT_Y		0
#define	POT_H		24
#define	POT_L		24


#define	XRS_BANK_SAMPLE	'xbks'
#define	MSG_N			'plut'

const 	rgb_color  	onc 	= {255,52,49};




JTrack::JTrack(BRect rect,int16 n):TrackBlock(rect,""),id(n),model(0)
{
	SetViewColor(bkColor);

	AddChild(volpot=new APot(BRect(545,POT_Y,545+POT_H,POT_Y+POT_L),"pippo",new BMessage(TRACK_VOL),new BMessage(TRACK_ON),0, 100, XUtils::GetBitmap(0),XUtils::GetBitmap(1)));
	AddChild(panpot=new APot(BRect(550+POT_H+3,POT_Y+3,550+POT_H+3+18,POT_Y+21),"pippo",new BMessage(TRACK_PAN),new BMessage(PAN_RESET),-100, 100,XUtils::GetBitmap(2),NULL));
	AddChild(brez= new BPictureButton(BRect(6,7,6+10,7+8),"_menu",XUtils::GetPicture(0),XUtils::GetPicture(1),new BMessage('rez'),B_TWO_STATE_BUTTON));

}

void
JTrack::RControl()
{
	AddChild(xtr=new XTrack(BRect(1+SBUTTON_W,0,1+SBUTTON_W+TRACK_LX,TRACK_LY-1),"init"));
	AddChild(vnc=new XNotesView( BRect(160,0,530,BUTTON_LY),id));
	TrackEnd *a,*b;
	AddChild(a=new VolView(BRect(0,Bounds().bottom+1,530,Bounds().bottom+10)));
	AddChild(b=new PianoControl(BRect(0,Bounds().bottom+11,530,Bounds().bottom+21),""));
	
	trackend_list.Add(a);
	trackend_list.Add(b);
	
	//Line position (new in 1.3)
	BRect	pos(volpot->Frame());
	pos.right=pos.left+36;
	pos.bottom=pos.top+22;
	AddChild(line_pos = new XDigit(pos, VID_EMPTY, "mixer_line", new BMessage(TRACK_ROUTELINE), 0,4));
	line_pos->MoveBy(-50,0);
}

void
JTrack::InvokeRename()
{
		if(xtr)
			xtr->SendRenameMessage();
}

void
JTrack::MessageReceived(BMessage* message)
{
	switch(message->what)
	{

	case NOTIFY_NOTE:
		Window()->PostMessage(message,trackend_list[0]);
		Window()->PostMessage(message,trackend_list[1]);
	break;
	case XRS_BANK_SAMPLE:	
	{
		entry_ref	ref;
		message->FindRef("refs",&ref);
		TrackManager::Get()->RefReceived(ref,this,message);
	}
	break;
		
	case XRS_SIMPLE_DATA:
	 {
	 	entry_ref	ref;
		message->FindRef("refs",&ref);
		TrackManager::Get()->RefReceived(ref,this);
	 }
	break;
	case TRACK_ON:
		myTrack->setOn(volpot->IsOn());
	break;
	case TRACK_VOL:
		myTrack->SetGain((float)volpot->Value()/100.0f);
	break;
	case TRACK_PAN:
		myTrack->SetPan((float)panpot->Value()/100.0f);
	break;
	case	B_SIMPLE_DATA:
	break;
	case PAN_RESET:
		myTrack->SetPan(0.0f);
		panpot->SetValue(0);
	break;
	case FORCE_ROUTELINE:
		if(message->FindInt32("be:value")!= line_pos->GetValue()) ((BControl*)line_pos)->SetValue(message->FindInt32("be:value"));
	break;
	
	case TRACK_ROUTELINE:
		int32 value;
		if (message->FindInt32("be:value", &value) == B_OK) {
			myTrack->setRouteLine(value);
			myTrack->RouteLineChanged(value);
		}
	break;
	
	case JMSG_NAME_SET:
		
		if(myTrack==NULL) return;
		
		const char* name;
		message->FindString("_value", &name);
		
		
		myTrack->LockName(false);
		
		if(strlen(name)>0) 
			{
				SetName(name);	
				myTrack->LockName(true);
			}
			
			else
				{
					myTrack->ResetName();
					xtr->SetName(myTrack->getName());
				}
		if(TrackManager::Get()->getCurrentJTrack()==this)
			TrackManager::Get()->ResetPanel(myTrack);
		//Window()->CurrentFocus()->SetEventMask(B_KEYBOARD_EVENTS);
		Window()->WindowActivated(true);
	break;
	case JMSG_NAME_NOTSET:
		Window()->WindowActivated(true);
	break;
	
	case CHANGE_NAME:
		{
		
		BPoint f,t;
		
		message->FindPoint("from", &f);
		message->FindPoint("to", &t);
		
		/*  Disabling KeyBoard for MainWindow */
		Window()->WindowActivated(false);
		new TextControlFloater(BRect(f,t),B_ALIGN_LEFT,be_plain_font,myTrack->getName(),this,new BMessage(JMSG_NAME_SET),new BMessage(JMSG_NAME_NOTSET));
		SetSelected(true);
		}
	break;
	default:
		
		TrackBlock::MessageReceived(message);
		
		break;
	}

}

void
JTrack::AttachedToWindow()
{
	xtr->SetTarget(Window());
	volpot->SetTarget(this);
	panpot->SetTarget(this);
	brez->SetTarget(this);
	line_pos->SetTarget(this);	
	TrackBlock::AttachedToWindow();
}
void
JTrack::Init(BMessage *m)
{
	// check this.
	xtr->Init(m);
}

void
JTrack::ResetToTrack(Pattern* p , Track* tr, int16 beatDivision)
{
	
	myTrack=tr;
	model=tr->getModel();
	int poz;
	
	Refresh(p, beatDivision);
	
	xtr->SetPadColor(tr->GetPreferredPadColor());
	
	if(myPat->getNumberNotes()>NUM_NOTES) poz=myPat->getNumberNotes();
		else
	poz=NUM_NOTES;
	
		
	ResizeTo(160+23*poz+2+107,Frame().Height());
		
	
	vnc->ResizeTo(23*myPat->getNumberNotes()+2,vnc->Frame().Height());
	
	VolView* v=(VolView*)trackend_list[0];
	
	if(tr->SupportPanNote())
		if( tr->SupportSustainNote())
			v->SupportMultiMode(SUSTAIN_VIEW);
		else
			v->SupportMultiMode(PAN_VIEW);
	
	TrackEnd *a,*b;
	a=trackend_list[0];
	b=trackend_list[1];
	
	a->ResizeTo(160+23*myPat->getNumberNotes(),a->Frame().Height());
	b->ResizeTo(160+23*myPat->getNumberNotes()+B_V_SCROLL_BAR_WIDTH,b->Frame().Height());
		
	volpot->MoveTo(170+23*poz,volpot->Frame().top);	
	panpot->MoveTo(170+23*poz+32,panpot->Frame().top);		
	
	line_pos->MoveTo(panpot->Frame().right+10,0);
	((BControl*)line_pos)->SetValue(myTrack->getRouteLine());
	
	volpot->SetValue( (int32)(myTrack->Gain() * 100.0f));
	volpot->SetOn(myTrack->isOn());
	
	panpot->SetValue((int32)(myTrack->Pan() * 100.0f));
	

	SetName(tr->getName());
	
	
}

void
JTrack::SetName(const char* t)
{
	if(myTrack==NULL) return;
		
	if(Window()->Lock())
	{
		myTrack->setName(t);
		xtr->SetName(myTrack->getName());
		Window()->Unlock();
	}
}

void
JTrack::Refresh(Pattern* p, int16 beatDivision)
{
	myPat=p; 
	vnc->Reset(p, beatDivision);
	
	TrackEnd* a = trackend_list[0];
	TrackEnd* b = trackend_list[1];

	((PianoControl*)b)->Reset(p);
	((VolView*)a)->Reset(p);

}
void
JTrack::Select()
{
	xtr->Select();
}
int
JTrack::getModel()
{
	return model;
}
void
JTrack::Deselect()
{
	xtr->Deselect();
}
void
JTrack::Mute(bool b,bool c)
{

	//ATTENZIONE! ->Mute() si comporta non come sembra..	
	
	if(!c)
		volpot->SetOn(!volpot->IsOn());
	
	else
		volpot->SetOn(b);
	
	
	myTrack->setOn(volpot->IsOn());
}

