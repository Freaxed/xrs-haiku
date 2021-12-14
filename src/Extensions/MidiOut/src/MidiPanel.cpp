#include 	"MidiPanel.h"
#include	"XDigit.h"
#include	"MidiOutTrack.h"
#include 	"Xed_Utils.h"
#include	<StringView.h>
#include	<Message.h>
#include	<MenuItem.h>
#include	"APot.h"

#define	SET_MIDICH	'stmc'
#define	SET_CC		'cccc'
#define	SET_CC_VALUE 'ccva'
#define	SET_PG		'prch'
#define	SET_PG_MENU	'prcm'
#define	SET_SEND_PLAY 'ssop'


#include "GeneralMidi.h"
BMenu *submenu;

MidiPanel::MidiPanel():PlugPanel(){

	
	myTrack=NULL;
	
	BRect sbox(8,18,172,50);
	BBox *sampler= new BBox(sbox ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	BRect r(sampler->Bounds());
	r.InsetBy(4,4);
	r.right-=50;
	r.bottom-=2;
	sampler->AddChild(new BStringView(r,"",T_MIDIOUT_OUTPUT));
	sampler->AddChild(ch=new XDigit(BRect(120,5,120+36,5+21),new BMessage(SET_MIDICH),1,16));
	
	//
	sbox.OffsetBy(0,50);
	BBox *sampler2= new BBox(sbox ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	r=sampler2->Bounds();
	r.InsetBy(4,4);
	r.right-=50;
	r.bottom-=2;
	
	
	menu=new BMenu(instruments[0]);
	sampler2->AddChild(field=new BMenuField(r,"","",menu));
	sampler2->AddChild(ch2=new XDigit(BRect(120,5,120+36,5+21),new BMessage(SET_PG),0,127));
	
	sbox.OffsetBy(0,30);
	AddChild(new BStringView(sbox,"",T_MIDIOUT_CONTROLLERS));
	
	//conrollers
	BMessage *mess;
	int32 s=64;
	sbox.OffsetBy(7-s,50);
	sbox.right=sbox.left+22;
	sbox.bottom=sbox.top+22;
	
	r=sbox;
	for(int i=0;i<3;i++){
	r.OffsetBy(s,0);
	mess=new BMessage(SET_CC_VALUE);
	mess->AddInt16("id",i);
	my_cont[i].value=new APot(r,"",mess,NULL,0,127,XUtils::GetBitmap(24),NULL);
	AddChild(my_cont[i].value);
	}
	sbox.OffsetBy(-7,24);
	r=sbox;
	r.right=r.left+36;
	
	for(int i=0;i<3;i++){
	r.OffsetBy(s,0);
	mess=new BMessage(SET_CC);
	mess->AddInt16("id",i);
	my_cont[i].number=new XDigit(r,mess,0,127);
	AddChild(my_cont[i].number);
	}
	
	//AddChild(my_cont[0].value);
	//AddChild(my_cont[0].number);
	sbox.OffsetBy(7,35);
	r=sbox;
	r.left=sampler2->Frame().left;
	r.right=sampler2->Frame().right;
	
	AddChild(send_ck=new BCheckBox(r,"",T_MIDIOUT_SEND_CC,new BMessage(SET_SEND_PLAY)));

	AddChild(sampler2);
	AddChild(sampler);

	
}

void
MidiPanel::Reset(Track* tr){
	if(tr){
		SetTitle(tr->getName());
		if(Window()->Lock()){
		myTrack=(MidiOutTrack*)tr;
		ch->SetValue(myTrack->GetChannel()+1,false);
		ch2->SetValue(myTrack->GetProgramChange(),false);
		menu->Superitem()->SetLabel(instruments[ch2->GetValue()]);
		
		send_ck->SetValue(myTrack->GetSendOnPlay());
		
		for(int i=0;i<3;i++){
		my_cont[i].value->SetValue(myTrack->GetCCValue(i));
		my_cont[i].number->SetValue(myTrack->GetCCNumber(i),false);
		}
		Window()->Unlock();
		}
	}
	else
	myTrack=NULL;
	PlugPanel::Reset(tr);
	
};
void
MidiPanel::AttachedToWindow()
{
	ch->SetTarget(this);
	ch2->SetTarget(this);
	menu->SetTargetForItems(this);
	send_ck->SetTarget(this);
	
	for(int i=0;i<3;i++){
	my_cont[i].value->SetTarget(this);
	my_cont[i].number->SetTarget(this);
	}
	
	BMessage *mess;
		
	for(int i=0;i<16;i++){
	
	submenu=new BMenu(groups[i]);
	
	for(int j=0;j<8;j++){
	mess=new BMessage(SET_PG_MENU);
	mess->AddInt32("program_change",i*8+j);
	submenu->AddItem(new BMenuItem(instruments[i*8+j],mess));
	
	}
	menu->AddItem(submenu);
	submenu->SetTargetForItems(this);
	field->SetDivider(0);
  }
}
void
MidiPanel::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case SET_SEND_PLAY:
		if(!myTrack) return;
		myTrack->SetSendOnPlay(send_ck->Value());
		break;
		case SET_MIDICH:
		if(!myTrack) return;
		myTrack->SetChannel(ch->GetValue()-1);
		
		break;
		case SET_PG:
		if(!myTrack) return;
		myTrack->SetProgramChange(ch2->GetValue());
		
		menu->Superitem()->SetLabel(instruments[ch2->GetValue()]);
				
		break;
		case SET_PG_MENU:
			ch2->SetValue(msg->FindInt32("program_change"));
		break;
		case SET_CC:
		if(!myTrack) return;	
			myTrack->SetCCNumber(msg->FindInt16("id"),my_cont[msg->FindInt16("id")].number->GetValue());
		break;
		case SET_CC_VALUE:
		if(!myTrack) return;
			myTrack->SetCCValue(msg->FindInt16("id"),my_cont[msg->FindInt16("id")].value->Value());
		break;
		default:
		PlugPanel::MessageReceived(msg);
		break;
	}
	
}
