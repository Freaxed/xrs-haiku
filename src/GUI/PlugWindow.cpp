/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
#include <Alert.h>

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
PlugWindow::SetPrograms(const char *name, BMenu* prog)
{
	if (NULL == prog)
		return;
	Lock();
	bar->AddItem(nameMenu=new BMenuItem(name,new BMessage(X_WIN_ZOOM)));
	nameMenu->SetTarget(this);
	
	BMenu	  *men=new BMenu(T_VSTWIN_PRESETS);
	BMenuItem *prgs=new BMenuItem(prog);

	men->AddItem(prgs);
	men->AddItem(presetz=new BMenu("User"));
	men->AddSeparatorItem();
	men->AddItem(new BMenuItem(T_MENU_SAVE_AS,new BMessage(X_SAVE_PRESET)));

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
		} else 
			BAlert("Saving preset", "Error in saving preset!", "Ok");
		
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
		}	else 
			BAlert("Loading preset", "Error in loading preset!", "Ok");
	}
	else
	if(msg->what==X_SAVE_LOST){
	
		if(MainWindow::Get()->Lock()){
		
			MainWindow::Get()->WindowActivated(true);
			MainWindow::Get()->Unlock();
		} 
	
	}
	else BWindow::MessageReceived (msg); 
	
}
void
PlugWindow::SaveSettings(BMessage& m)
{
	m.AddFloat("Xpos",Frame().left);
	m.AddFloat("Ypos",Frame().top);}


void
PlugWindow::LoadSettings(BMessage& msg)
{
		float x,y;
		if(msg.FindFloat("Xpos", &x) == B_OK &&  msg.FindFloat("Ypos", &y) == B_OK)
		{
			if(Lock()){
				MoveTo(x, y);
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
	TextControlFloater *tf=new TextControlFloater(BRect(BRect(x,y,x+200,y+20)),
							"",this,new BMessage(X_SAVE_PRESET_OK),new BMessage(X_SAVE_LOST));
	tf->SetLook(B_FLOATING_WINDOW_LOOK);
	tf->SetTitle(T_VSTWIN_NAME);
}

//--
