/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Window.h"
#include "Alert.h"

#define PREFS_H 250
#define PREFS_L 400

#include "Panels.h"
#include <TabView.h>
#include <StringView.h>
#include <Box.h>
#include <Button.h>
#include <FilePanel.h>
#include <Application.h>
#include <Path.h>

#include "StatusBar.h"
#include "RadioButton.h"
#include "Xed_Utils.h"
#include "GlobalDef.h"
#include "Configurator.h"
#include "JFileManager.h"
#include "GfxMsg.h"
#include "Song.h"
#include "TextView.h"
#include "XDigit.h"
#include "BzWindow.h"
#include "locale.h"


static		entry_ref rif;


#define BOXSPACE	10.0f
#define OFFSET		5.0f


#include <ControlLook.h>
#include <FindDirectory.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <LayoutBuilder.h>

#define kMsgAddFolder 		'addf'
#define kMsgRemoveFolder 	'remf'
#define kMsgSelect			'self'



#define kMsgCancel 			'canj'
#define kMsgSave 			'resj'
#define kVstFolders 		"vstfolders"

#ifdef MEDIA_BROWSER
 #define kMsgAddFolderMB 		'addm'
 #define kMsgRemoveFolderMB 		'remm'
 #define kMsgSelectMB			'selm'
#endif

//Preferences
void
Panels::showPrefsPanel()
{
	BzWindow *win=new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"Preferences", B_FLOATING_WINDOW, 0);
//	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE|B_NOT_ZOOMABLE);
	float padding = be_control_look->DefaultItemSpacing();
	
	
	// VST
	BBox* foldersBox = new BBox("VST");
	foldersBox->SetBorder(B_NO_BORDER);

	BButton* addButton = new BButton("add", "Add", new BMessage(kMsgAddFolder));

	BButton* fRemove = new BButton("remove", "Remove", new BMessage(kMsgRemoveFolder));

	fRemove->SetEnabled(false);

	BListView* fFoldersListView = new BListView(BRect(), "listFolders",B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL,
                B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE | B_FULL_UPDATE_ON_RESIZE);

	fFoldersListView->SetSelectionMessage(new BMessage(kMsgSelect));
	BScrollView* foldersScrollView = new BScrollView("folderScroll", fFoldersListView, B_FOLLOW_ALL_SIDES,0,true,true);
	foldersScrollView->SetExplicitMinSize(BSize(be_plain_font->Size() * 30, 250));

	BLayoutBuilder::Group<>(foldersBox, B_VERTICAL, padding)
			.SetInsets(padding)
			.Add(new BStringView("txt","Additional folders to parse for VST effects and instruments"))
			.AddGroup(B_HORIZONTAL, padding)
				.SetInsets(0)
				.Add(foldersScrollView)
					.AddGroup(B_VERTICAL, padding / 2, 0.0f)
						.SetInsets(0)
						.Add(addButton)
						.Add(fRemove)
						.AddGlue();	
			
	// END VST

#ifdef MEDIA_BROWSER	
	// Media Browser
	BBox* mbBox = new BBox("Media Browser");
	mbBox->SetBorder(B_NO_BORDER);
	
	BButton* mbAdd 	  = new BButton("mb_add", "Add", new BMessage(kMsgAddFolderMB));
	BButton* mbRemove = new BButton("mb_remove", "Remove", new BMessage(kMsgRemoveFolderMB));
	mbRemove->SetEnabled(false);
	
	BListView* mbListView = new BListView(BRect(), "listFolders",B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL,
                B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE | B_FULL_UPDATE_ON_RESIZE);
    
    mbListView->SetSelectionMessage(new BMessage(kMsgSelectMB));
    
    BScrollView* mbScroll = new BScrollView("mbScroll", mbListView, B_FOLLOW_ALL_SIDES,0,true,true);
	mbScroll->SetExplicitMinSize(BSize(be_plain_font->Size() * 30, 250));
	
	
	BLayoutBuilder::Group<>(mbBox, B_VERTICAL, padding)
		.SetInsets(padding)
		.Add(new BStringView("txt","MediaBrowser folders to scan for samples files"))
		.AddGroup(B_HORIZONTAL, padding)
			.SetInsets(0)
			.Add(mbScroll)
				.AddGroup(B_VERTICAL, padding / 2, 0.0f)
					.SetInsets(0)
					.Add(mbAdd)
					.Add(mbRemove)
					.AddGlue();						
	// END MEDIA BROWSER
#endif
					
	BTabView* tabView = new BTabView("tabview", B_WIDTH_FROM_LABEL);				
	tabView->AddTab(foldersBox);

#ifdef MEDIA_BROWSER
	tabView->AddTab(mbBox);
#endif

	BBox* fButtonsBox = new BBox("buttons");
	fButtonsBox->SetBorder(B_NO_BORDER);


	BButton* fCancel = new BButton("cancel",  "Cancel", new BMessage(kMsgCancel));
	BButton* fSave = new BButton("save", "Save", new BMessage(kMsgSave));


	fCancel->SetEnabled(true);
	fSave->SetEnabled(true);


	BLayoutBuilder::Group<>(fButtonsBox, B_HORIZONTAL, padding)
			.SetInsets(padding)
			.Add(new BStringView("txt","You need to restart XRS to apply these settings"))
			.AddGlue()
			.AddGroup(B_HORIZONTAL, padding / 2, 0.0f)
					.Add(fCancel)
					.Add(fSave)
					.AddGlue();

	BLayoutBuilder::Group<>(win, B_VERTICAL, 0)
			.SetInsets(0, B_USE_WINDOW_SPACING)
			.Add(tabView)
			.AddStrut(B_USE_DEFAULT_SPACING)
			.Add(fButtonsBox);

	if (fRemove->Bounds().Width() > fSave->Bounds().Width())
			fSave->SetExplicitMinSize(BSize(fRemove->Bounds().Width(), B_SIZE_UNSET));
	else
			fRemove->SetExplicitMinSize(BSize(fSave->Bounds().Width(), B_SIZE_UNSET));

	win->ResizeToPreferred();
	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	
	entry_ref rif;
	for(int i=0; Config()->FindRef(kVstFolders, i, &rif) == B_OK; i++)
	{
		fFoldersListView->AddItem(new BStringItem(BPath(&rif).Path()));
	}	
	
	win->Show();
	win->RedirectMessages(Panels::msgPrefs, (void*)win);

}


bool
Panels::msgPrefs(BMessage* message,void* cookies)
{
	BzWindow *win=(BzWindow*)cookies;
	static  BMessenger messenger(NULL, win);
	static  BFilePanel openpanel(B_OPEN_PANEL, &messenger, &rif,B_DIRECTORY_NODE,false,NULL,NULL,true,true);
	switch(message->what)
	{
	  case kMsgSelect:
		{
			BListView *list = (BListView*)win->FindView("listFolders");
			BListItem* fSelected = list->ItemAt( list->CurrentSelection());
			BButton* button = (BButton*)win->FindView("remove");
			button->SetEnabled(fSelected != NULL);
        }
		break;
		case kMsgAddFolder:
		{
			openpanel.Show();
		}
		break;
		case kMsgRemoveFolder:
		{
			BListView *list = (BListView*)win->FindView("listFolders");
			BListItem* fSelected = list->ItemAt( list->CurrentSelection());
			list->RemoveItem(fSelected);
			BButton* button = (BButton*)win->FindView("remove");
			button->SetEnabled(false);			
		}
		break;
		case kMsgCancel:
			win->PostMessage(B_QUIT_REQUESTED);
		break;
		case kMsgSave:
		{
			BListView *list = (BListView*)win->FindView("listFolders");
			Config()->RemoveName(kVstFolders);
			for (int i=0;i<list->CountItems();i++)
			{
				entry_ref	rif;
				BStringItem* fItem = (BStringItem*)list->ItemAt(i);
				get_ref_for_path(fItem->Text(),&rif);
				Config()->AddRef(kVstFolders, &rif);				
			}
			
			win->PostMessage(B_QUIT_REQUESTED);	
		}

		break;
		case B_REFS_RECEIVED:
		{
			entry_ref rif;
			if(message->FindRef("refs", &rif) == B_OK)
			{
				BListView *list = (BListView*)win->FindView("listFolders");
				list->AddItem(new BStringItem(BPath(&rif).Path()));
			}
		}
		break;
		default:
			return true;
		break;
	}
	
	return true;
}

void
Panels::showSettings(Song* curSong)
{
	BBox 			*box,*box2;
	XDigit		*a,*b;
	BTextView	*tw;
	BCheckBox	*cb;
	
	BzWindow *win=new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_FLOATING_WINDOW,0);
	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE);
	
	win->AddChild(box=new BBox(win->Bounds(),"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_PLAIN_BORDER));
	BRect r(box->Bounds());
	r.InsetBy(BOXSPACE,BOXSPACE);
	r.bottom -= BOXSPACE+20;
	box->AddChild(box2=new BBox(r,T_SETTINGS_TITLE,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_FANCY_BORDER));
	box2->SetLabel(T_SETTINGS_TITLE);
	
	r=box2->Bounds();
	r.right =r.Width()/2.0f;
	r.left +=20;
	
	r.top=r.top+20;
	r.bottom =r.top+20;
	
	box2->AddChild(new BStringView(r,"",T_SETTINGS_SONG_DESCRIPTION,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.left=r.right+1;
	r.right *=2;
	r.right -=20;
	box2->AddChild(cb=new BCheckBox(r,"",T_SETTINGS_POPUP_ONLOAD,NULL));
	
	r=box2->Bounds();
	r.InsetBy(20,20);
	r.top +=21;
	r.bottom -=30;
	BRect x;
	x.top=0;
	x.left=0;
	x.right=r.Width();
	x.bottom=r.Height();
	box2->AddChild(tw=new BTextView(r,"",x,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r=box2->Bounds();
	r.right =(r.Width())/2.0f;
	r.left +=20;
	r.top = r.bottom-39;
	r.bottom=r.top+32;
		
	BBox *sampler= new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW, B_FANCY_BORDER);		
	x=sampler->Bounds();
	x.left=x.right-40;
	x.right=x.left+36;
	x.top=5;
	x.bottom=x.top+22;
	sampler->AddChild(a=new XDigit(x, VID_EMPTY, "sampler?", NULL, 2, 8));
	box2->AddChild(sampler);
	x=sampler->Bounds();
	x.InsetBy(4,4);
	x.right -=45;
	sampler->AddChild(new BStringView(x,"set",T_SETTINGS_NOTES_PER_MEASURE,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.left=r.right+5;
	r.right *=2;
	r.right -=20;
	sampler=new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW, B_FANCY_BORDER);
	x=sampler->Bounds();
	x.left=x.right-40;
	x.right=x.left+36;
	x.top=5;
	x.bottom=x.top+22;
	sampler->AddChild(b=new XDigit(x, VID_EMPTY,"sampler2?", NULL, 2,8));
	box2->AddChild(sampler);
	x=sampler->Bounds();
	x.InsetBy(4,4);
	x.right -=45;
	sampler->AddChild(new BStringView(x,"set",T_SETTINGS_BEAT_DIVISION,B_FOLLOW_NONE,B_WILL_DRAW));
	r=box2->Frame();
	r.top=r.bottom+5;
	r.bottom =r.top+20;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));
		
	box->AddChild(new BButton(r,"",T_GEN_CANCEL,new BMessage('Canc'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.right = r.left - OFFSET;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));

	box->AddChild(new BButton(r,"set",T_GEN_OK,new BMessage('Ok'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	tw->SetStylable(true);
	tw->SetText(curSong->getDescription());
	cb->SetValue(curSong->popupdesc);
	
	a->UpdateValue((int32)curSong->GetBeats(),  true);
	b->UpdateValue((int32)curSong->GetBeatDivision(), true);
	
	BMessage *out=new BMessage();
	out->AddPointer("win",(void*)win);
	out->AddPointer("song",(void*)curSong);
	out->AddPointer("a",(void*)a);
	out->AddPointer("b",(void*)b);
	out->AddPointer("tw",(void*)tw);
	out->AddPointer("cb",(void*)cb);
	
	
	
	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	win->Show();
	win->RedirectMessages(Panels::msgSettings,(void*)out);

	
}
bool
Panels::msgSettings(BMessage* message,void* cookies)
{
	BzWindow *win;
	BMessage *x=((BMessage*)cookies);
	x->FindPointer("win",(void**)&win);

	
	switch(message->what)
	{
		
		case 'Canc':
		
			win->PostMessage(B_QUIT_REQUESTED);
				
		break;
		case 'Ok':
			{
				Song *curSong;
				x->FindPointer("song",(void**)&curSong);
				XDigit *a,*b;
				x->FindPointer("a",(void**)&a);
				x->FindPointer("b",(void**)&b);
				BTextView	*tw;
				BCheckBox	*cb;
				x->FindPointer("tw",(void**)&tw);
				x->FindPointer("cb",(void**)&cb);
					
				curSong->setDescription(tw->Text());
				curSong->popupdesc=(bool)cb->Value();
				curSong->setBeatInfo((int16)a->GetValue(), (int16)b->GetValue());
				be_app->PostMessage(new BMessage(SONG_RESET));
				win->PostMessage(B_QUIT_REQUESTED);
			}
		break;
		
		default:
			return true;
		break;
	}
	
	return true;
	
}

#include "PBox.h"
void
Panels::showExport(int k,int m)
{
	BzWindow *win=new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_FLOATING_WINDOW,0);
	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE);
	
	BBox *box;
	win->AddChild(box=new BBox(win->Bounds(),"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_PLAIN_BORDER));
	BRect r(box->Bounds());
	r.InsetBy(20,20);
	r.bottom -=30;
	
	BMessage fTemplate;
	BMessage fData;
	
	fTemplate.AddString("name", "Export Settings");
	
	BMessage s1;
	s1.AddString("name", "pattern");
	s1.AddString("description", "Section to export");
	s1.AddInt32("type", B_STRING_TYPE);
	s1.AddString("valid_value", "Current Pattern"); //0
	s1.AddString("valid_value", "Entire Song");     //1
	s1.AddString("default", 	"Current Pattern");
	
	BMessage s2;
	s2.AddString("name", "format");
	s2.AddString("description", "Format");
	s2.AddInt32("type", B_STRING_TYPE);
	s2.AddString("valid_value", "AIFF WAVE (32bit float)");
	s2.AddString("default", 	"AIFF WAVE (32bit float)");
	
	BMessage s3;
	s3.AddString("name", "empty_space");
	s3.AddString("description", "Time at the end (seconds)");
	s3.AddInt32("type", B_INT32_TYPE);
	s3.AddInt32("valid_value", 0);
	s3.AddInt32("valid_value", 1);
	s3.AddInt32("valid_value", 10);
	s3.AddInt32("valid_value", 30);
	s3.AddInt32("default", 	0);
	
	
	fTemplate.AddMessage("setting", &s1);
	fTemplate.AddMessage("setting", &s2);
	fTemplate.AddMessage("setting", &s3);
			
	PBox* pbox = new PBox(r, fTemplate, fData, "Export Settings");
	box->AddChild(pbox);

	r=pbox->Frame();
	r.top=r.bottom+5;
	r.bottom =r.top+20;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));


	box->AddChild(new BButton(r,"set",T_GEN_CANCEL,new BMessage('Canc'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.right = r.left - OFFSET;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));

	box->AddChild(new BButton(r,"set",T_GEN_EXPORT,new BMessage('Expt'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	win->Show();
	win->RedirectMessages(Panels::msgExport,(void*)pbox);
	
	
}

bool
Panels::msgExport(BMessage* message,void* cookies)
{
	PBox *x=((PBox*)cookies);
	switch(message->what)
	{		
		case 'MENU':
		{
			BMenuItem* item;
			if (message->FindPointer("source", (void**)&item) == B_OK)
			{
				if (BString(item->Menu()->Name()).Compare("pattern") == 0)
				{
					int32 index = 0;
					if (message->FindInt32("index", &index) == B_OK)
					{
						BMenuField* empty_space = (BMenuField*)x->Window()->FindView("empty_space_control");
						empty_space->SetEnabled((index == 0));
					}
				}
			}
		}
		break;
		case 'Canc':
			x->Window()->PostMessage(B_QUIT_REQUESTED);
		break;
		case 'Expt':
		{			
			BMessage *info=new BMessage('expt');
			x->GetData(info);
			JFileManager::Get()->ExportWave(info);
		}
		break;	
		default:
			return true;
		break;
	}
	return true;
}

#include "LoadingError.h"
#include "Colors.h"
void
Panels::showLoadingError()
{
	const ErrorList& list = LoadingError::GetErrorList();

	if (list.CountItems() == 0)
		return;

	BBox 			*box;
	BTextView		*tw;

	
	BzWindow* win = new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_FLOATING_WINDOW,0);
	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE);
	
	win->AddChild(box=new BBox(win->Bounds(),"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_PLAIN_BORDER));
	
	BRect r(box->Bounds());
	r.InsetBy(20,20);
	r.right  -= B_V_SCROLL_BAR_WIDTH;
	r.bottom -= (30 + B_H_SCROLL_BAR_HEIGHT);

	BRect x;
	x.top    = 0;
	x.left   = 0;
	x.right  = r.Width();
	x.bottom = r.Height();

	tw = new BTextView(r, "set", x, B_FOLLOW_NONE, B_WILL_DRAW);
	box->AddChild(new BScrollView("textview_scroll", tw, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, true, true,B_FANCY_BORDER));
	
	r.top    = r.bottom + 10 + B_H_SCROLL_BAR_HEIGHT;
	r.bottom = r.top    + 20;
	r.left   = r.right  - 60 + B_V_SCROLL_BAR_WIDTH;
	
	box->AddChild(new BButton(r,"set","Oh!",new BMessage(B_QUIT_REQUESTED),B_FOLLOW_NONE,B_WILL_DRAW));

	tw->SetStylable(true);
	tw->SetFontAndColor(be_bold_font, B_FONT_ALL, &Red);
	tw->Insert("ERROR IN LOADING FILE!");
	tw->SetFontAndColor(be_plain_font, B_FONT_ALL, &Black);
	tw->Insert("\n\n");
	
	for (int i=0; i<list.CountItems();i++)
	{
		ErrorItem* element = list.ItemAt(i);
		tw->Insert(element->who.String());
		tw->Insert(": ");
		tw->Insert(element->what.String());
		tw->Insert("\n");
		tw->SetFontAndColor(be_plain_font, B_FONT_ALL, &Blue);
		tw->Insert(element->solution.String());
		tw->SetFontAndColor(be_plain_font, B_FONT_ALL, &Black);
		tw->Insert("\n\n");
	}

	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	win->Show();
}
