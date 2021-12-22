/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SamplerPanel.h"
#include "Sample.h"
#include "SamplerTrackBoost.h"
#include "GlobalDef.h"
#include "GfxMsg.h"
#include "SamplerTrack.h"
#include "Xed_Utils.h"
#include "sampler_locale.h"

#include "XDigit.h"
#include "XHost.h"

#define	REMOVE		'remv'
#define	REMOVEALL	'rema'
#define	LOADEXT		'loae'
#define 	REL_MSG 	'note'
#define 	REV_ON		'reo'
#define	PIT_ON 		'pio'
#define	BOOST_ON 	'boo'
#define	MOD			'mod'

//const 	rgb_color  	red 	={253,148,106,255};
//const 	rgb_color  	cacca 	={80,80,250,255};


SamplerPanel::SamplerPanel(SamplerTrackBoost* sb) :
	PlugPanel(),sampTrack(NULL),booster(sb)
	{
	
	
	
	
	
	BBox *sam_box= new BBox(BRect(10,150+17,171,210+17) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	
	BBox *sampler= new BBox(BRect(8,18,172,50) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
		
	menu=new BMenu(" ");
	menu->AddItem(booster->getMenu());
	menu->AddItem(new BMenuItem(T_SAMPLER_LOAD,new BMessage(LOADEXT)));
	menu->AddItem(new BMenuItem(T_SAMPLER_REMOVE,new BMessage(REMOVE)));
	menu->AddItem(new BMenuItem(T_SAMPLER_REMOVE_ALL,new BMessage(REMOVEALL)));
	BRect r(sampler->Bounds());
	r.InsetBy(4,4);
	field=new BMenuField(r,""," ",menu);

		
	
	
	
	pit_box= new BBox(BRect(8,70-13,172,102-13), "");
	
	r=(pit_box->Bounds());
	r.InsetBy(4,4);
	r.right-=50;
	r.top+=2;
	
	pit_box->AddChild(pit_ck=new BCheckBox(r,"",T_SAMPLER_STRECH,new BMessage(PIT_ON)));
	pit_ck->SetValue(0);
	pit_ck->SetFontSize(12);
	pit_box->AddChild(shift=new XDigit(BRect(120,5,120+36,5+21), VID_EMPTY, "sampler_shift_xdigit", new BMessage(MOD),1,32));
	
	r=(pit_box->Frame());
	r.OffsetBy(0,38);
	AddChild(pit_box);
	
	pit_box= new BBox(r, "2");
	
	r=(pit_box->Bounds());
	r.InsetBy(4,4);
	r.right-=50;
	r.top+=2;
	
	pit_box->AddChild(boost_ck=new BCheckBox(r,"",T_SAMPLER_BOOST,new BMessage(BOOST_ON)));
	boost_ck->SetValue(0);
	boost_ck->SetFontSize(12);
	pit_box->AddChild(depth=new XDigit(BRect(120,5,120+36,5+21), VID_EMPTY, "sampler_boost", new BMessage(REL_MSG),1,4));
	
	r=(pit_box->Frame());
	r.OffsetBy(0,38);
	AddChild(pit_box);
	
	
	pit_box= new BBox(r, "3");
	
	r=(pit_box->Bounds());
	r.InsetBy(4,4);
	r.right-=50;
	r.top+=2;
		
	pit_box->AddChild(rev=new BCheckBox(r,"rev_check",T_SAMPLER_REVERSE,new BMessage(TRACK_REV)));
	rev->SetValue(0);
	rev->SetFontSize(12);
	AddChild(pit_box);
		
	
	sw=new SampleView(BRect(1,1,159,58));
	sam_box->AddChild(sw);

	sampler->AddChild(field);
	
	AddChild(sampler);
	AddChild(sam_box);
	
	my_sample=NULL;
	rev->SetValue(false);
	menu->Superitem()->SetLabel(T_SAMPLER_NOSELECTED);
	
	
}

void
SamplerPanel::Reset(Track* trk){

	//qui magari un bel check dell'ID ??
	PlugPanel::Reset(trk);
	SetTrack((SamplerTrack*)trk);	
	
}
void
SamplerPanel::AttachedToWindow()
{
	PlugPanel::AttachedToWindow();
	depth->SetTarget(this);
	shift->SetTarget((BView*)this);
	pit_ck->SetTarget(this);
	rev->SetTarget(this);
	menu->SetTargetForItems(this);
	boost_ck->SetTarget(this);
	
	field->SetDivider(0);//field->Bounds().Width()-25);
	sw->SetViewBitmap(XUtils::GetBitmap(18));
}
void
SamplerPanel::SetTrack(SamplerTrack *tr)
{
	if(!Window()) return;
	
	sampTrack=tr;
	
	if(Window()->Lock()){
	if(tr==NULL || tr->getSample()==NULL)
	{
		
		sw->Init(0,NULL,0,0);
		shift->UpdateValue(16, true);
		pit_ck->SetValue(false);
		boost_ck->SetValue(false);
		//field->SetLabel("No sample selected");
		menu->Superitem()->SetLabel(T_SAMPLER_NOSELECTED);
		depth->UpdateValue(1, true);
		Window()->Unlock();
		return;
	}
	else
	{
		SetTitle(tr->getName());
		my_sample=tr->getSample();
		sw->Init(my_sample->channels,my_sample->wave_data,my_sample->frames,tr->isReversed());
		//field->SetLabel(my_sample->name);
		menu->Superitem()->SetLabel(my_sample->name);
		shift->UpdateValue(tr->getResample(), true);
		pit_ck->SetValue(tr->isResampleEnable());
		boost_ck->SetValue(tr->isBoostEnable());
		rev->SetValue(tr->isReversed());
		depth->UpdateValue((int32)tr->amp, true);
		sw->SetAmp(tr->amp);

		
	}
	
	Window()->Unlock();
	}
}

void
SamplerPanel::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		
		case BOOST_ON:
			if(sampTrack==NULL) return;
			sampTrack->setBoostEnable((bool)boost_ck->Value());						
			if(!boost_ck->Value()) 
			{
				sampTrack->amp=1.0;
				sw->SetAmp(sampTrack->amp);
				return;
			}
			//else continue (without break!)
		case REL_MSG:
			if(sampTrack==NULL) return;
			
			if(!sampTrack->isBoostEnable()) return;
			
			sampTrack->amp=(float)depth->GetValue();
			sw->SetAmp(sampTrack->amp);
			
		break;
		
	 	case TRACK_SAMP_EXT:
	 		booster->ChangeSample(message->FindInt16("sample"));//ok
	 	break;
	 	case MOD:
	 		if(sampTrack==NULL) return;
	 		XHost::Get()->SendMessage(X_LockSem,NULL);
	 			sampTrack->setResample(shift->GetValue());
	 		XHost::Get()->SendMessage(X_UnLockSem,NULL);
	 	break;
	 	case PIT_ON:
	 		if(sampTrack==NULL) return;
	 		XHost::Get()->SendMessage(X_LockSem,NULL);
	 			sampTrack->setResampleEnable((bool)pit_ck->Value());
	 		XHost::Get()->SendMessage(X_UnLockSem,NULL);
	 	break;
	 	case TRACK_REV:
			if(sampTrack==NULL) return;
			sampTrack->setReversed(rev->Value());
			sw->SetReversed(rev->Value());
		
		break;
		case LOADEXT:
			booster->LoadSample();
		break;
		case REMOVEALL:
			booster->RemoveAll();
		break;
		case REMOVE:
			booster->RemoveSample(((SamplerTrack*)sampTrack)->getSample());
		break;
		case B_REFS_RECEIVED:	//ok
		{
			
			entry_ref	ref;
			if(message->FindRef("refs",&ref)==B_OK)
			{
					booster->RefReceived(ref,sampTrack);
					booster->RefreshSelected();
			}
		}			
		break;	
	 	default:
			PlugPanel::MessageReceived(message);
		break;
	}
}



