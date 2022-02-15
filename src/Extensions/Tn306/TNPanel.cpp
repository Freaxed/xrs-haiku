/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TNPanel.h"
#include "knob303.h"
#include "TNTrack.h"
#include "Xed_Utils.h"
#include "XHost.h"
#include "TNTrackBoost.h"
#include "TextControlFloater.h"

static const float MIN_WIDTH = 192.f;
static const float MIN_HEIGHT = 204.0f;
static const float XPOS = 100.0f;
static const float YPOS = 200.0f;

#define	PRESET_REVISION		1 //NB: >>0!
#define PRESET_DIRECTORY	"Tn306Extension"

#define	X_SAVE_PRESET		'xspt'
#define	X_LOAD_PRESET		'xlpt'
#define	X_SAVE_PRESET_OK 	'xspo'
#define	X_SAVE_LOST			'xslo'


TNPanel::TNPanel(TNTrackBoost* booster):PlugPanel(),fBooster(booster)
{	
			//menu bar
	BRect r(Bounds());
	BMenuBar	*barra=new BMenuBar(r,"barra");
	men=new BMenu(T_VSTWIN_PRESETS);
		
	presetz=new BMenu(T_VSTWIN_USER);
	XUtils::FillPresetsMenu(PRESET_DIRECTORY,presetz,X_LOAD_PRESET);
		
	men->AddItem(presetz);
	men->AddSeparatorItem();
	men->AddItem(new BMenuItem(T_MENU_SAVE_AS,new BMessage(X_SAVE_PRESET)));
		
	barra->AddItem(men);
			
	barra->ResizeToPreferred();
	AddChild(barra);
	//
	float top=barra->Bounds().bottom+1;
		
	AddChild(knobs=new TB303Knobs(BRect(0,top,MIN_WIDTH,top+MIN_HEIGHT)));	
	fTrack=NULL;
}

void
TNPanel::AttachedToWindow()
{
	//BBitmap	*tile=XUtils::MakeBitmapSkin("TNPanel");
	//knobs->SetViewBitmap(tile);
	
	men->SetTargetForItems(this);
	presetz->SetTargetForItems(this);
	
	PlugPanel::AttachedToWindow();
	
}

void
TNPanel::ResetToTrack(Track *track)
{
	fTrack = dynamic_cast<TNTrack*>(track);
	
	if (fTrack != NULL)
	{
		SetTitle(track->getName());
		if(Window()->Lock()){
			knobs->Reset(fTrack);
			Window()->Unlock();
		}
	}
	PlugPanel::ResetToTrack(track);
}

void
TNPanel::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case X_SAVE_PRESET:
		{
			BRect frame(Bounds());
			frame=ConvertToScreen(Bounds());
			float x=frame.left+((frame.right-frame.left)/2.0)-100;
			float	y=frame.top+50;
			XHost::Get()->SendMessage(X_MainWindowDeactivate,0);
			
			TextControlFloater *tf=new TextControlFloater(BRect(BRect(x,y,x+200,y+20)),B_ALIGN_LEFT,be_plain_font,"",this,new BMessage(X_SAVE_PRESET_OK),new BMessage(X_SAVE_LOST));
			tf->SetLook(B_FLOATING_WINDOW_LOOK);
			tf->SetTitle(T_VSTWIN_NAME);
		}
		break;
		case X_SAVE_LOST:
			XHost::Get()->SendMessage(X_MainWindowActivate,0);
			break;
		case X_SAVE_PRESET_OK:
		{
			XHost::Get()->SendMessage(X_MainWindowActivate,0);
			
			if(!fTrack || !fBooster) return;
			
			BString nome;
			msg->FindString("_value",&nome);
			if(nome.Length()<=0) return; 
			 BMessage	set;
			 fBooster->SaveTrackSettings((Track*)fTrack,&set);
			
			set.AddString("name",nome.String());
			set.AddInt16("revision",PRESET_REVISION);
			
			if(XUtils::SavePreset(PRESET_DIRECTORY,nome.String(),&set)){
				BMenuItem* ni;
				presetz->AddItem(ni=new BMenuItem(nome.String(),new BMessage(X_LOAD_PRESET)));
				ni->SetTarget(this);
			}
		}
		break;
		case X_LOAD_PRESET:
		{
			XHost::Get()->SendMessage(X_MainWindowActivate,0);
			
			if(!fTrack || !fBooster) return;
			
			int pos=msg->FindInt32("index");
			BMessage	set;
			BMenuItem	*item=presetz->ItemAt(pos);
			
			if(XUtils::LoadPreset(PRESET_DIRECTORY,item->Label(),&set)){
				if(set.FindInt16("revision")==PRESET_REVISION){
					fBooster->LoadTrackSettings((Track*)fTrack,&set);
					ResetToTrack((Track*)fTrack);
				}
			}	
		}
		break;
		default:
		 PlugPanel::MessageReceived(msg);
		break;
	}
}
