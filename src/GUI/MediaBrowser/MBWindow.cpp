/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "MBWindow.h"
#include "XListView.h"
#include "XListItem.h"
#include "Xed_Utils.h"
#include "WindowManager.h"
#include "locale.h"


#include <Application.h>
#include <stdio.h>
#include <StorageKit.h>
#include <ScrollView.h>
#include <string.h>
#include <Bitmap.h>

#define SELECTED_MSG 	'slm'
#define INVOCATION_MSG	'inv'
#define	X_WINDOW_ZOOM	'xwzc'

#define	EXPAND	'expa'
#define	RELOAD	'relo'


MBWindow::MBWindow() :
	XrsWindow(BRect(450,130,680,364),T_BROWSER_TITLE,B_DOCUMENT_WINDOW,0) //B_FLOATING_WINDOW,B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_NOT_RESIZABLE|B_NOT_MINIMIZABLE|B_AVOID_FOCUS) //
{
	SetName("mediabrowser_");
	
	// Here we're going to get the mini fIcon from a specific mime type
	BRect _rect(0.0, 0.0, 15.0, 15.0); 
	fIcon 	= new BBitmap(_rect, B_CMAP8);
	fIwave	= new BBitmap(_rect, B_CMAP8);
	
	BMimeType mime("application/x-vnd.Be-directory");
	mime.GetIcon(fIcon, B_MINI_ICON);	
	
	BMimeType mime2("audio");
	mime2.GetIcon(fIwave, B_MINI_ICON);	
		
	fList = new XListView(BRect(0,22,Bounds().Width()-B_V_SCROLL_BAR_WIDTH,Bounds().Height()-B_V_SCROLL_BAR_WIDTH));
	
	fList->SetInvocationMessage(new BMessage(INVOCATION_MSG));
	fList->SetSelectionMessage(new BMessage(SELECTED_MSG));
		
	BRect r(Bounds());
	r.bottom = 20;
	BBox *toolbar;
	AddChild(toolbar= new BBox(r,"toolbar",B_FOLLOW_NONE,B_WILL_DRAW,B_PLAIN_BORDER));
	r=BRect(2,1,20,18);
	
	
	
	toolbar->AddChild(XUtils::EasyButton(r,0,new BMessage(EXPAND),toolbar));
	
	r.OffsetBy(20,0);
	
	toolbar->AddChild(XUtils::EasyButton(r,1,new BMessage(RELOAD),toolbar));


	BScrollView *sw;
	AddChild(sw= new BScrollView("name",fList,B_FOLLOW_ALL_SIDES,0,true,true) );
	
	fSound = NULL;
	
	sw->SetViewColor(toolbar->ViewColor());
	
	
	
	LoadConfig();
	
	WindowManager::Get()->RegisterMe(this,T_BROWSER_TITLE);
	
	_addFolders();
}
void
MBWindow::_addFolders()
{	
	entry_ref s;
	XUtils::GetXRSDirectoryEntry(&s,"Samples");
	BEntry e(&s,true);
		AddFolder(s);
}

MBWindow::~MBWindow(){ 
	SaveConfig(); 
	WindowManager::Get()->UnregisterMe(this);
}

void
MBWindow::AddFolder(entry_ref ref)
{
	BListItem* samples;
	fList->AddItem(samples = new IconListItem(fIcon, ref.name, 0, false));
	samples->SetExpanded(true);
	ParseDir(&ref,samples);
}
void
MBWindow::ParseDir(entry_ref *ref,BListItem* folder)
{
	BDirectory dir(ref);
	BEntry e;
	entry_ref rif;
	BListItem* samples;
	
	while (dir.GetNextEntry(&e,true) == B_OK)
	{
		e.InitCheck();
		if(e.IsFile())
		{
			e.GetRef(&rif);
			//TODO: check filetype before adding?
			fList->AddUnder(new XListItem(fIwave,&rif),folder);
		}
		else
		if(e.IsDirectory())
		{
			e.GetRef(&rif);
			fList->AddUnder(samples = new IconListItem(fIcon, rif.name, 0, false),folder);
			samples->SetExpanded(false);
			ParseDir(&rif,samples);
		}
	}
}

bool
MBWindow::QuitRequested()
{
	WindowManager::Get()->Hide(this);
	return false;
}

void
MBWindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case  SELECTED_MSG:
			Activate(true);
			break;
		case RELOAD:
			for(int i=0;i<fList->CountItems();i++)
			 	delete fList->ItemAt(i);
			fList->MakeEmpty();
			_addFolders();
			break;
		case EXPAND:
			for(int i=0;i<fList->CountItems();i++)
			 	fList->Collapse(fList->ItemAt(i));
			break;
		case  INVOCATION_MSG:
			PlaySelectedSound();
			break;
		case X_WINDOW_ZOOM:
			if(Bounds().Width()==19)
				ResizeTo(680-450,Bounds().Height());
			else
				ResizeTo(19,Bounds().Height());
			break;
		default:
			XrsWindow::MessageReceived(message);
	}
	
}

void
MBWindow::PlaySelectedSound()
{
	XListItem *item = dynamic_cast<XListItem*>(fList->ItemAt(fList->CurrentSelection(0)));
			
	if (!item)
		return;
				
	entry_ref 	ref = item->getRef();
	BPath p(&ref);
	
	if(fSound)
	{
		fSound->StopPlaying();
		delete fSound;
		fSound = NULL;			
	} 
		
	fSound = new BSimpleGameSound(&ref);
	fSound->StartPlaying();		
}
