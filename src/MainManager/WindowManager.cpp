/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include	"WindowManager.h"
#include	"XrsWindow.h"
#include	"locale.h"

#include	<malloc.h>
#include	<stdio.h>
#include	<Message.h>
#include "Log.h"

WindowManager::WindowManager():BList(10),BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE,WINSTATE)
{
	menu=new BPopUpMenu("menu");
	menu->SetRadioMode(false);
	
	menu->AddSeparatorItem();
	
	BMessage	*msg=new BMessage(WINSTATE);
	BMenuItem	*item;
	msg->AddBool("close_all",true);
	menu->AddItem(item=new BMenuItem(T_MENU_HIDE_ALL,msg,'-'));
	item->SetTrigger('C');
}


WindowManager*
WindowManager::Get() {
 	static	WindowManager*	instance = NULL;
	if (instance == NULL) 
			instance = new WindowManager();
	return instance;

}

WindowManager::~WindowManager(){

	for (int i = 0; i < CountItems(); i++)
	{
		delete (token*)ItemAt(i);
	}
	MakeEmpty();
}

void
WindowManager::_switch(BWindow* mw,int32 pos){

	bool hidden=false;
	
	if (mw == NULL) 
		return;
	
	if (mw->Lock())
	{
		if ((hidden = mw->IsHidden())) 
			mw->Show();
		else 
			mw->Hide();
		
		mw->Unlock();
	}
	
	if (pos<0 || pos>menu->CountItems()-1) 
		return;
	
	menu->ItemAt(pos)->SetMarked(hidden);
}
	
void
WindowManager::Switch(BWindow* mw){

	for (int i=0; i<CountItems();i++)
	{
		token	*t=(token*)ItemAt(i);
		if (t && t->pointer == mw)
		{
			_switch(mw,i);
			break;
		}
	}
	
}	

void 
WindowManager::SaveSettings(BMessage& msg)
{
	BMessage wMan;
	LogTrace("WindowManager::SaveSettings -> %d windows", CountItems());
	for(int i=0;i<CountItems();i++)
	{		
		token* t= (token*) ItemAt(i);
		BMessage regWindow;
		regWindow.AddString("Name", t->reg_name.String());
		regWindow.AddBool("Show", !t->pointer->IsHidden());
		wMan.AddMessage("Window", &regWindow);
	}
	msg.RemoveName("WindowManager");
	msg.AddMessage("WindowManager", &wMan);
}

void 
WindowManager::LoadSettings(BMessage& msg)
{
	BMessage wMan;
	if (msg.FindMessage("WindowManager", &wMan) == B_OK)
	{
		int j=0;
		BMessage regWindow;
		while(wMan.FindMessage("Window", j++, &regWindow) == B_OK)
		{
			bool show = regWindow.GetBool("Show", true);
			BString name = regWindow.GetString("Name", "___null___");
			for(int i=0;i<CountItems();i++)
			{
				token* t= (token*) ItemAt(i);
				if (t->reg_name == name)
				{
					show ? Show(t->pointer) : Hide(t->pointer);
				}
			}
		}
	}
}

void
WindowManager::Show(BWindow* mw){

	if (mw->Lock())
	{
		if (!mw->IsHidden()) 
		{
			mw->Activate();
			mw->Unlock();
			return;
		}
		else
			mw->Show();
		
		 for(int i=0;i<CountItems();i++)
		 {
		  token	*t=(token*)ItemAt(i);
		  if(t && t->pointer==mw){
		 	menu->ItemAt(i)->SetMarked(true);
		 	break;
		  }
		 }
		mw->Unlock();
	}	
		
}
void
WindowManager::Hide(BWindow* mw){

	if(mw->Lock())
	{
		if(mw->IsHidden()) 
		{
			mw->Unlock();
			return;
		}
		else
		mw->Hide();
		 for(int i=0;i<CountItems();i++)
		 {
		  token	*t=(token*)ItemAt(i);
		  if(t && t->pointer==mw){
		 	menu->ItemAt(i)->SetMarked(false);
		 	break;
		  }
		 }
		mw->Unlock();
	}	
		
}

void
WindowManager::Switch(int32 pos){

	if(pos<0 || pos>CountItems()-1) return;
	
	token	*t=(token*)ItemAt(pos);
	
	_switch(t->pointer,pos);
}	
void
WindowManager::RegisterMe(BWindow* win,const char *str){

	token	*t=new token;
	t->reg_name=str;	
	t->pointer=win;
	
	int32 pos=CountItems();

	BMenuItem*	item;
	
	AddItem((void*)t,pos);
	menu->AddItem(item=new BMenuItem(str,new BMessage(WINSTATE)),pos); //first
	
	if(win->Lock())
	{
		if(win->IsHidden()) 
			item->SetMarked(false);
		else 
			item->SetMarked(true);
		win->Unlock();
	}

	LogTrace("WindowManager: registering window %s (%d)", str, CountItems());
}
void
WindowManager::UnregisterMe(BWindow* win){

	for(int i=0;i<CountItems();i++)
	{
		token	*t=(token*)ItemAt(i);
		if(t && t->pointer==win){
		
		LogTrace("WindowManager: un-registering window %s  (%d)",  t->reg_name.String(), CountItems());
			RemoveItem(t);
			menu->RemoveItem(i);
			break;
			}
	}
	
	

}

BPopUpMenu* 
WindowManager::getMenu(){ 
	return menu;
}

void
WindowManager::CloseAll()
{
	for(int i=0;i<CountItems();i++)
	{
		token	*t=(token*)ItemAt(i);
		if(t) Hide(t->pointer);
		menu->ItemAt(menu->CountItems()-1)->SetMarked(false);
	}
	
}
filter_result
WindowManager::Filter(BMessage *message, BHandler **target)
{
	int32 	index;
	BWindow	*win;
	if(message->GetBool("close_all", false))
	{
		CloseAll();
	}
	else
	if(message->FindInt32("index",&index)==B_OK)
	{
		Switch(index);
	}
	else
	if(message->FindPointer("win",(void**)&win)==B_OK)
	{
		Switch(win);
	}
	
	return B_SKIP_MESSAGE;
}
