/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"PlugWindow.h"

#include 	<Box.h>
#include 	<Menu.h>
#include 	<MenuBar.h>
#include	<MenuItem.h>
#include	<ScrollBar.h>
#include	<Button.h>
#include	<TextControl.h>
#include 	<stdio.h>

#include	"MainWindow.h"
#include	"TextControlFloater.h"
#include	"WindowManager.h"
#include 	"VSTItem.h"
#include 	"VSTParamsView.h"
#include	"BzWindow.h"
#include	"VstManager.h"


#include "locale.h"

#define	X_WIN_ZOOM			'xwzo'
#define	X_SAVE_PRESET		'xspt'
#define	X_LOAD_PRESET		'xlpt'
#define	X_SAVE_PRESET_OK 	'xspo'
#define	X_SAVE_LOST			'xslo'


PlugWindow::PlugWindow(VSTItem* p,bool scroll) :
	BWindow(BRect(250,30,280,60),"notset", B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE),
	big(true),plugin(p)
{
	if(p == NULL) 
		return;
	

	VstManager::Get()->setPresetsPath(plugin);

	config = (VSTParamsView*)plugin->Configure();

	SetControls(config, false);
	
	SetPrograms(plugin->EffectName(),config->CreateMenu());	
}

PlugWindow::~PlugWindow()
{
	if(config)
		RemoveChild(config);
}

bool
PlugWindow::QuitRequested()
{
	WindowManager::Get()->Hide(this);
	return false;
}
void
PlugWindow::SetControls(BView* conf,bool scr)
{
	if(Lock())
	{
		bar=new BMenuBar(BRect(0,0,30,20),"Bar");
		AddChild(bar);
		
		BBox* box=new BBox(BRect(0,bar->Frame().Height()+1,conf->Bounds().Width(),bar->Frame().Height()+conf->Bounds().Height()),"Boxer",B_FOLLOW_NONE);
		ResizeTo(conf->Bounds().Width(),conf->Bounds().Height()+bar->Frame().Height());
	
		box->AddChild(conf);
		if(scr)
		{
			ResizeTo(conf->Bounds().Width()+B_V_SCROLL_BAR_WIDTH+1,300);
			AddChild(new BScrollBar(BRect(conf->Bounds().Width()+1,bar->Frame().Height(),conf->Bounds().Width()+1+B_V_SCROLL_BAR_WIDTH,300),"scoller",conf,0,conf->Bounds().Height()+bar->				Frame().Height()-300,B_VERTICAL));	
			
		}
		
			AddChild(box);
			Unlock();
			
			bigH=Frame().Height();
			bigW=Frame().Width();
		}
			
}

void
PlugWindow::SetPrograms(const char *name,BMenu* prog)
{
	if (NULL == prog)
		return;
	Lock();
	bar->AddItem(nameMenu=new BMenuItem(name,new BMessage(X_WIN_ZOOM)));
	nameMenu->SetTarget(this);
	
	BMenu	  *men=new BMenu(T_VSTWIN_PRESETS);
	BMenuItem *prgs=new BMenuItem(prog);
	//if(prog!=NULL) prgs->AddMenu(prog);
	men->AddItem(prgs);
	/* creazione del menu presez..*/
	
	
	men->AddItem(presetz=new BMenu("User"));
	men->AddSeparatorItem();
	men->AddItem(new BMenuItem(T_MENU_SAVE_AS,new BMessage(X_SAVE_PRESET)));
	//men->AddItem(new BMenuItem("Reload",new BMessage(X_RELOAD_PRESETS)));
	
	//men->SetRadioMode(true);
	
	/* caricamento presets list*/
	//BList	*tlist=new BList(5);
	VstManager::Get()->FillPresetsMenu(plugin,presetz,X_LOAD_PRESET);
	
	bar->AddItem(men);
	Unlock();
}



void
PlugWindow::SetState(bool s,float force)
{
	if (!s)	
	{
 		smallH=bar->Frame().Height();
		
		if (force < 0) 
			smallW=nameMenu->Frame().Width();
		else
			smallW=force;
			
		ResizeTo(smallW,smallH);
		big=false;
	}
	else
	{
		ResizeTo(bigW,bigH);
		big=true;
	}
}
void
PlugWindow::MessageReceived(BMessage* msg)
{
	if(msg->what=='xwzo')
	{
		SetState(!big);
		
	}
	else
	if(msg->what==X_SAVE_PRESET)
	{
		
		SetName();
		
	}
	else
	if(msg->what==X_SAVE_PRESET_OK)
	{
		if(MainWindow::Get()->Lock()){
			MainWindow::Get()->WindowActivated(true);
			MainWindow::Get()->Unlock();
		} 
		BString nome;
		msg->FindString("_value",&nome);
		if(nome.Length()<=0) return; 
		BMessage	set;
		plugin->SavePreset(&set);
		set.AddString("name",nome.String());
		set.AddString("plugin_name",plugin->EffectName());
		
		if(VstManager::Get()->SavePreset(plugin,nome.String(),&set)){
			presetz->AddItem(new BMenuItem(nome.String(),new BMessage(X_LOAD_PRESET)));
		}
		
	}
	else
	if(msg->what==X_LOAD_PRESET)
	{
		if(MainWindow::Get()->Lock()){
		
			MainWindow::Get()->WindowActivated(true);
			MainWindow::Get()->Unlock();
		} 
		int pos=msg->FindInt32("index");
		BMessage	set;
		BMenuItem	*item=presetz->ItemAt(pos);
		if(VstManager::Get()->LoadPreset(plugin,item->Label(),&set)){
			plugin->LoadPreset(&set);
			if(config) config->ResetUIFromPlugin();
		}	
	}
	else
	if(msg->what==X_SAVE_LOST){
	
		if(MainWindow::Get()->Lock()){
		
			MainWindow::Get()->WindowActivated(true);
			MainWindow::Get()->Unlock();
		} 
	
	}
	/*else
	if(msg->what==X_RELOAD_PRESETS)
	{
		
		while(presetz->CountItems())
		{
			BMenuItem* item=presetz->ItemAt(0);
			if(!item) break;
			presetz->RemoveItem(item);
			delete item;	
		}
		
		MainManager::Get()->FillPresetsMenu(plugin,presetz,X_LOAD_PRESET);
		
	}*/
	else BWindow::MessageReceived (msg); 
	
}
void
PlugWindow::SavePref(BMessage* m)
{
	m->AddFloat("Xpos",Frame().left);
	m->AddFloat("Ypos",Frame().top);
	m->AddFloat("Ysize",nameMenu->Frame().Width());
	m->AddBool("big",GetState());
}


void
PlugWindow::LoadPref(BMessage* msg)
{
		float x;
		bool b;
		if(msg->FindFloat("Xpos",&x)==B_OK)
		{
			if(Lock()){
			MoveTo(x,msg->FindFloat("Ypos"));
			msg->FindFloat("Ysize",&smallW);
			
			if(msg->FindBool("big",&b)==B_OK)
				SetState(b,smallW);
				
			config->ResetUIFromPlugin();
			Unlock();
			}
		}
}

void
PlugWindow::SetName()
{
	float x=Frame().left+((Frame().right-Frame().left)/2.0)-100;
	float	y=Frame().top+50;
	
	if(MainWindow::Get()->Lock()){
		
			MainWindow::Get()->WindowActivated(false);
			MainWindow::Get()->Unlock();
		} 
	TextControlFloater *tf=new TextControlFloater(BRect(BRect(x,y,x+200,y+20)),B_ALIGN_LEFT,be_plain_font,
							"",this,new BMessage(X_SAVE_PRESET_OK),new BMessage(X_SAVE_LOST));
	tf->SetLook(B_FLOATING_WINDOW_LOOK);
	tf->SetTitle(T_VSTWIN_NAME);
}

//--
