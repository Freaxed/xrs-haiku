#include "SFWindow.h"
#include <Application.h>
#include <Messenger.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include	"XHost.h"
#include	"APot.h"
#include	"Xed_Utils.h"
#include "XDigit.h"

#include <Box.h>
#include <Slider.h>
#include <InterfaceKit.h>
#include <FilePanel.h>
#include <Path.h>
#include <Entry.h>

#include 	"sfs_locale.h"
#include	"SFSTrackBoost.h"

extern 	SFSTrackBoost*	fluid_booster;

#define	WWIN	390	
#define	HWIN	310

BFilePanel*	openpanel;
BMessenger* mess;

extern	XFSynth			*ptheSynth;

#define MULT 100

			
SFWindow::SFWindow() :
	BWindow(BRect(450,30,450+WWIN,30+HWIN),"SoundFont", B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE)
{
	SetFlags (Flags () | B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_NOT_RESIZABLE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS);
	
	
	InitGUI();
	XHost::Get()->SendMessage(X_RegWindow,(void*)this,(void*)this->Name());
	
	mess= new BMessenger(this);
	openpanel=new BFilePanel(B_OPEN_PANEL,mess,NULL,0,false,new BMessage('loax'),NULL,true,true);	
	openpanel->SetMessage(new BMessage('loax'));		
}

void
SFWindow::Reset()
{
	if(Lock())
	{	
		pot_gain->SetValue(ptheSynth->GetGain() * MULT);
		pot_reverb_roomsize->SetValue(ptheSynth->GetReverbRoomSize() * MULT); 	
		pot_reverb_damping->SetValue(ptheSynth->GetReverbDamp() * MULT); 			
		pot_reverb_width->SetValue(ptheSynth->GetReverbWidth()  * MULT);			
		pot_reverb_level->SetValue(ptheSynth->GetReverbLevel()  * MULT); 		
	
		ck_reverb->SetValue(ptheSynth->IsReverbOn() ? 1 : 0);
		
		
		int chorus_type = ptheSynth->GetChorusType();
		if (chorus_type>1) 
			chorus_type = 1;
			
		field_chorus_type->Menu()->ItemAt(chorus_type)->SetMarked(true);
		pot_chorus_level->SetValue(ptheSynth->GetChorusLevel()*10);
		pot_chorus_depth->SetValue(ptheSynth->GetChorusDepth()*10);
		digit_chorus_n->UpdateValue(ptheSynth->GetChorusNr(), false);				
		pot_chorus_speed->SetValue(ptheSynth->GetChorusSpeed()*100.);
		
		
		ck_chorus->SetValue(ptheSynth->IsChorusOn() ? 1 : 0);
		
		
		str_name->SetText(ptheSynth->GetSoundFontName()); //fixme use 'Leaf!'
	
		Unlock();
	}
}
void
SFWindow::info(void* data, char* name, int option)
{
//	printf("name %s \t\t option %d\n",name,option);
//	fluid_settings_foreach_option((fluid_settings_t*)data,name,NULL,optioninfo);
//	int ret=fluid_settings_is_realtime((fluid_settings_t*)data,name);
//	printf("\t REALTIME %d\n",ret);
		
}
void
SFWindow::optioninfo(void* data, char* name, char* option)
{
	printf("\t\t\t name %s \t option %s\n",name,option);
}

bool
SFWindow::QuitRequested()
{
	XHost::Get()->SendMessage(X_Switch,(void*)this);
	return false;
}

void
SFWindow::set_reverb()
{
	ptheSynth->SetReverb((float)pot_reverb_roomsize->Value()/100.,
				(float)pot_reverb_damping->Value()/100.,
				(float)pot_reverb_width->Value()/100.,
				(float)pot_reverb_level->Value()/100.);
			
}

void
SFWindow::set_chorus()
{
	ptheSynth->SetChorus(
				digit_chorus_n->GetValue(),
				(float)pot_chorus_level->Value()/10.,
				(float)pot_chorus_speed->Value()/100.,
				(float)pot_chorus_depth->Value()/10.,
				chorus_type);
			
}
void
SFWindow::MessageReceived(BMessage* message)
{
	int val;
	switch(message->what)
	{
		case 'gain':
			val=message->FindInt32("be:value");
			ptheSynth->SetGain((float)val/100.0);
		break;
		//reverb
		case 'revo':
			
			val=message->FindInt32("be:value");
			ptheSynth->SetReverbOn((bool)val);
		break;
		case 'revX':
			set_reverb();
		break;
		//chorus
		case 'choo':
			
			val=message->FindInt32("be:value");
			ptheSynth->SetChorusOn((bool)val);
		break;
		case 'choX':
		
			set_chorus();
		break;
		case 'chot':
			
			chorus_type=(int)message->FindInt32("index");
			set_chorus();
		break;
		case 'load':
			
			
			openpanel->Show();
		break;
		case 'loax':
		{
			
			entry_ref ref;
				
			if(message->FindRef("refs",&ref)==B_OK)
			{
				BEntry r(&ref);
				BPath p(&r);
				fluid_booster->LoadSoundFont(p.Path());
			}
			
			//delete mess;				
		}
		break;
		default:
			BWindow::MessageReceived(message);
		break;
	
	}	
}

void
SFWindow::InitGUI()
{
	BBox *sampler;
	BRect ir(0,0,0,0);
	
	BBox *box2=new BBox(Bounds(),"ciao",B_FOLLOW_ALL,0,B_PLAIN_BORDER);
	AddChild(box2);
	
	BRect r(8,0,172,50-18);
	
	r.OffsetBy(10,10);
	sampler= new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);
	
	ir=sampler->Bounds();
	ir.InsetBy(4,4);
	ir.right-=50; 
	ir.bottom-=6;
	sampler->AddChild(str_name=new BStringView(ir,"",T_SFS_NO_SF2));
	sampler->AddChild(XUtils::ToolBarButton(BRect(156-24,26-22,156,26), 1,"TIP_OPEN", new BMessage('load'), B_ONE_STATE_BUTTON, box2));
	box2->AddChild(sampler);
	
	
	r=box2->Bounds();
	r.top +=51;
	r.left +=10;
	r.bottom -=10;
	r.right =r.left+180;
	BBox *box=new BBox(r,"ciao");//,B_FOLLOW_NONE,0);
	box->SetLabel(ck_reverb=new BCheckBox(ir,T_SFS_REVERB,T_SFS_REVERB,new BMessage('revo')));
	box2->AddChild(box);
	

	r=BRect(8,0,172,50-18);
		
	r.OffsetBy(0,41);
	pot_reverb_level=createCanvas(r,T_SFS_LEVEL,'revX',box);
		
	r.OffsetBy(0,41);
	pot_reverb_width=createCanvas(r,T_SFS_WIDTH,'revX',box);
	
	r.OffsetBy(0,41);
	pot_reverb_damping=createCanvas(r,T_SFS_DAMPING,'revX',box);
		
	r.OffsetBy(0,41);
	pot_reverb_roomsize=createCanvas(r,T_SFS_ROOMSIZE,'revX',box);	
	
		
	
	//chorus
	r=box->Frame();
	r.OffsetBy(r.right+1,0);
	
	box=new BBox(r,"chorus");
	box2->AddChild(box);
	
	r=BRect(8,0,172,50-18);
	
	
	r.OffsetBy(r.right+11+15,10);
	pot_gain=createCanvas(r,T_SFS_GAIN,'gain',box2,0,100);
	pot_gain->SetValue((int32)(100.*0.8));
	
	
	r=BRect(8,0,172,50-18);
	box->SetLabel(ck_chorus=new BCheckBox(ir,T_SFS_CHORUS,T_SFS_CHORUS,new BMessage('choo')));
	
	r.OffsetBy(0,41);
	pot_chorus_level=createCanvas(r,T_SFS_LEVEL,'choX',box);
	
	
	r.OffsetBy(0,41);
	sampler= new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
				B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);
	
	ir=sampler->Bounds();
	ir.InsetBy(4,4);
	ir.right-=50; 
	ir.bottom-=6;
	sampler->AddChild(new BStringView(ir,"",T_SFS_NR));
	sampler->AddChild(digit_chorus_n=new ADigit(BRect(120,5,120+36,5+21), new BMessage('choX'),1,99));
	box->AddChild(sampler);
	
	
	r.OffsetBy(0,41);
	pot_chorus_depth=createCanvas(r,T_SFS_DEPTH,'choX',box);
	
	
	r.OffsetBy(0,41);
	pot_chorus_speed=createCanvas(r,T_SFS_SPEED,'choX',box,30,500);
	
	
	r.OffsetBy(0,41);
	sampler= new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
				B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);
	
	ir=sampler->Bounds();
	ir.InsetBy(4,4);
	BMenu *menu=new BMenu(T_SFS_WAVE_UNSET);
	menu->AddItem(new BMenuItem(T_SFS_WAVE_SINE,new BMessage('chot')));
	menu->AddItem(new BMenuItem(T_SFS_WAVE_TRIANGLE,new BMessage('chot')));
	menu->SetLabelFromMarked(true);
	field_chorus_type=new BMenuField(ir,"",T_SFS_WAVE_TYPE,menu);
	sampler->AddChild(field_chorus_type);
	box->AddChild(sampler);
}
APot*
SFWindow::createCanvas(BRect r,const char*txt,int32 mess,BView* view,int min,int max)
{
	APot *pot;
	BBox *sampler= new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
				B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);
	
	//BRect sbox=sampler->Frame();
	//sbox.OffsetBy(0,50);
	//sampler= new BBox(sbox ,"", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	r=sampler->Bounds();
	r.InsetBy(4,4);
	r.right-=50;
	r.bottom-=6;
	sampler->AddChild(new BStringView(r,"",txt));
	sampler->AddChild(pot=new APot(BRect(156-22,26-22,156,26),txt,new BMessage(mess),NULL,min,max,XUtils::GetBitmap(24),NULL));
	
	view->AddChild(sampler);
	pot->SetTarget(this);
	return pot;
}
