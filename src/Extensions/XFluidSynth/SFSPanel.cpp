#include 	"SFSPanel.h"
#include	"SFSTrack.h"
#include	"Xed_Utils.h"
#include	<StringView.h>
#include 	"XHost.h"
#include	"APot.h"
#include  "XDigit.h"
#include  "SFWindow.h"
#include  "SFSTrackBoost.h"
#include "sfs_locale.h"

#define	SET_CHANNEL			'setc'
#define	SET_REVERB			'setr'
#define	SET_CHORUS			'seth'




#define	SHOW_WINDOW	'sviw'

extern	fluid_settings_t* settings;
		
extern 	SFSTrackBoost*	fluid_booster;

extern	SFWindow*	sfwin;
extern	XFSynth			*ptheSynth;


SFSPanel::SFSPanel():PlugPanel(){


	myTrack=NULL;
	sampler_t= new BBox(BRect(8,18,172,50) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	
		

	menu=fluid_booster->getBankMenu();
	
	BRect r(sampler_t->Bounds());
	r.InsetBy(4,4);
	r.right -=40;
	field=new BMenuField(r,"","",menu);
	sampler_t->AddChild(field);
	
	
	
	AddChild(sampler_t);
	
	r.right +=40;
	r.left  = r.right-24;
	but=r;
	
	BRect sbox(sampler_t->Frame());
	sbox.OffsetBy(0,50);
	BBox* sampler= new BBox(sbox ,"", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	r=sampler->Bounds();
	r.InsetBy(4,4);
	r.right-=50;
	r.bottom-=6;
	sampler->AddChild(new BStringView(r,"",T_SFS_CHANNEl));
	sampler->AddChild(ch=new ADigit(BRect(120,5,120+36,5+21), new BMessage(SET_CHANNEL),1,16));
	
	AddChild(sampler);
	
	sbox=sampler->Frame();
	sbox.OffsetBy(0,50);
	sampler= new BBox(sbox ,"", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	r=sampler->Bounds();
	r.InsetBy(4,4);
	r.right-=50;
	r.bottom-=6;
	sampler->AddChild(new BStringView(r,"",T_SFS_REVERB));
	sampler->AddChild(reverb=new APot(BRect(156-22,26-22,156,26),"",new BMessage(SET_REVERB),NULL,0,127,XUtils::GetBitmap(24),NULL));
	
	AddChild(sampler);
	
	sbox=sampler->Frame();
	sbox.OffsetBy(0,50);
	sampler= new BBox(sbox ,"", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	r=sampler->Bounds();
	r.InsetBy(4,4);
	r.right-=50;
	r.bottom-=6;
	sampler->AddChild(new BStringView(r,"",T_SFS_CHORUS));
	sampler->AddChild(chorus=new APot(BRect(156-22,26-22,156,26),"", new BMessage(SET_CHORUS), NULL, 0,127,XUtils::GetBitmap(24),NULL));
	//sampler->AddChild(chorus=new XDigit(BRect(120,5,120+36,5+21),new BMessage(SET_CHORUS),0,127));

	AddChild(sampler);
}

void
SFSPanel::Refresh() {
	if(myTrack)
		ResetToTrack(myTrack);

}



void
SFSPanel::ResetToTrack(Track* tr){
	if(tr){
	
		SetTitle(tr->getName());
		
		if(Window()->Lock()){
			
			myTrack=(SFSTrack*)tr;
			ch->UpdateValue(myTrack->GetChannel()+1,false);
			
			if(ptheSynth->IsValid())			
				menu->Superitem()->SetLabel(ptheSynth->GetChannelPresetName(myTrack->GetChannel()));
			else
				menu->Superitem()->SetLabel(T_SFS_NO_SF2);
			
			reverb->SetValue(myTrack->GetReverbSend());
			chorus->SetValue(myTrack->GetChorusSend());
		
		Window()->Unlock();
		}
	}
	else
		myTrack=NULL;
	
	PlugPanel::ResetToTrack(tr);
	
	
};
void
SFSPanel::AttachedToWindow()
{
	PlugPanel::AttachedToWindow();
	for(int i=0;i<menu->CountItems();i++)
	 menu->SubmenuAt(i)->SetTargetForItems(this);
	ch->SetTarget(this);
	reverb->SetTarget(this);
	chorus->SetTarget(this);
	field->SetDivider(0);
	sampler_t->AddChild(brez=XUtils::ToolBarButton( but, 8, "_winW", new BMessage(SHOW_WINDOW), B_ONE_STATE_BUTTON, sampler_t));
	brez->SetTarget(this);
}



void
SFSPanel::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case SHOW_WINDOW:		
			if(sfwin) XHost::Get()->SendMessage(X_Switch,(void*)sfwin);
		break;	
		case BANK_PRESET:
			if(myTrack==NULL || !ptheSynth->IsValid()) return;	
			BMenuItem* src;
			if(msg->FindPointer("source",(void**)&src)==B_OK)
					menu->Superitem()->SetLabel(src->Label());
				
			ptheSynth->ProgramSelect(myTrack->GetChannel(),msg->FindInt16("bank"),msg->FindInt16("num"));
		break;
		
		case SET_CHANNEL:
			if(myTrack==NULL || !ptheSynth->IsValid()) return;		
			myTrack->SetChannel(ch->GetValue()-1);
			menu->Superitem()->SetLabel(ptheSynth->GetChannelPresetName(myTrack->GetChannel()));
		break;
		case SET_REVERB:
			if(myTrack)
				myTrack->SetReverbSend(reverb->Value());
		break;
		case SET_CHORUS:
			if(myTrack)
				myTrack->SetChorusSend(chorus->Value());
		break;
		default:
			PlugPanel::MessageReceived(msg);
		break;
	}
	
}
