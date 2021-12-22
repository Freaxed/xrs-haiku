/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"MainWindow.h"
#include 	"XPanel.h"
#include 	"JuiceEngine.h"
#include 	"Xed_Utils.h"
#include	"GraphicsDef.h"
#include 	"GfxMsg.h"
#include 	"JFileManager.h"
#include 	"TracksPanel.h"
#include 	"MatrixWindow.h"
#include 	"TickView.h"
#include 	"XHost.h"
#include	"MixerWindow.h"
#include 	"TrackManager.h"
#include	"MeasureManager.h"
#include	"locale.h"
#include 	"PositionView.h"
#include 	"XrsMidiIn.h"
#include 	"version.h"
#include	"Song.h"
#include	"WindowManager.h"

#include	<Roster.h>
#include 	<stdio.h>
#include 	<Application.h>
#include 	<Path.h>
#include 	<Entry.h>
#include 	<Directory.h>

#define	MOUSE_WHEEL	1598904131


MainWindow*
MainWindow::Get()
{
	static	MainWindow*	instance = NULL;
	if (instance == NULL) 
			instance = new MainWindow();
	return instance;
}


MainWindow::MainWindow() :
	XrsWindow(BRect(WINDOW_X_POS,WINDOW_Y_POS,WINDOW_X_POS+WINDOW_XL,WINDOW_Y_POS+WINDOW_YL),"Main", B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_WILL_ACCEPT_FIRST_CLICK) //|B_AVOID_FOCUS)

{
	SetName("");
	
	CreateMenu();
	float menuY, menuX;
	menuBar->GetPreferredSize(&menuX,&menuY);
	BRect nb(Bounds());
	nb.top=menuY+1;
	
	
	fPanel = new XPanel(nb); 
	AddChild(fPanel);
	
	
	nb=fPanel->Bounds();
	nb.top = XPANEL_H+INFO_BAR_LY + 3;
	nb.right -= B_V_SCROLL_BAR_WIDTH;
	nb.bottom -= B_V_SCROLL_BAR_WIDTH; 
	
	fTracksPanel = new TracksPanel(nb);
	
	BScrollView		*scroll_view;
	scroll_view = new BScrollView("XRScrollView", fTracksPanel , B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS, true, true, B_FANCY_BORDER);
	fPanel->AddChild(scroll_view);

	//SplitPane *split=new SplitPane(nb,scroll_view,new BView(BRect(0,0,10,10),NULL,0,0),B_FOLLOW_ALL_SIDES);
	//split->SetViewInsetBy(BPoint(0,0));
	//fPanel->AddChild(split);	
		
	ticks=new TickView( BRect(0,XPANEL_H,WINDOW_XL-B_V_SCROLL_BAR_WIDTH,XPANEL_H+INFO_BAR_LY+2));
	fPanel->AddChild(ticks);
	
	ticks->AddChild(new PositionView( BRect(10,0,10+54+54,18),0));		

	LoadConfig();

	float a,b,c,d;
	GetSizeLimits(&a,&b,&c,&d);
	SetSizeLimits(100,b,100,d);
	
	SetWheelTarget(NULL);
	
	AddShortcut('-', 0, new BMessage('hide'));
}

void
MainWindow::CreateMenu(){

	BMenu		*menuEdit;
	BMenu		*menuNew;
	BMenu		*edit_track,*edit_mea;
	BMenuItem	*edit_prefs;
	
	BMenuItem	*quit_item;
	
	BRect dummy(0, 0, 0, 0);
	menuBar = new BMenuBar(dummy, "Full Menu Bar");
	
	
	menuBar->AddItem(menuFile=new BMenu(T_MENU_FILE));
	menuBar->AddItem(menuEdit=new BMenu(T_MENU_EDIT));
	
	menuFile->AddItem(menuNew=new BMenu(T_MENU_NEW));
	menuNew->AddItem(new BMenuItem(T_MENU_DEFAULT,new BMessage(MENU_NEW_DEFAULT)));
	menuNew->AddItem(new BMenuItem(T_MENU_EMPTY,new BMessage(MENU_NEW_EMPTY),'N'));
	
	menuFile->AddItem(new BMenuItem(T_MENU_OPEN,new BMessage(MENU_OPEN),'O'));
	//menuFile->AddItem(openRecent=new BMenu("Open recent"));
	
	menuFile->AddSeparatorItem();
	menuFile->AddItem(save_item=new BMenuItem(T_MENU_SAVE,new BMessage(MENU_SAVE),'S'));
	menuFile->AddItem(saveas_item=new BMenuItem(T_MENU_SAVE_AS,new BMessage(MENU_SAVEAS)));
	menuFile->AddItem(new BMenuItem(T_MENU_EXPORT_AS_WAVE,new BMessage(MENU_EXPORT)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(new BMenuItem(T_MENU_COLLECT_AS_ZIP,new BMessage(MENU_ZIP)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(new BMenuItem(T_MENU_ABOUT,new BMessage(B_ABOUT_REQUESTED)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(quit_item=new BMenuItem(T_MENU_QUIT,new BMessage(B_QUIT_REQUESTED),'Q'));	//Un po' brutale..
	
	menuFile->SetEnabled(true);
	
	menuFile->SetTargetForItems(be_app);
	menuNew->SetTargetForItems(be_app);
	quit_item->SetTarget(this);
	
	
	//BMenu *add_menu;
	
	menuEdit->AddItem(edit_track=new BMenu(T_MENU_TRACK));
	menuEdit->AddItem(edit_mea=new BMenu(T_MENU_MEASURE));
	
	menuEdit->AddItem(new BMenuItem(T_MENU_PLAYLIST,new BMessage(EDIT_MSG)));
	menuEdit->AddSeparatorItem();
	menuEdit->AddItem(new BMenuItem(T_MENU_SONG_SETTINGS,new BMessage(MENU_SETTINGS)));
	menuEdit->AddSeparatorItem();
	menuEdit->AddItem(edit_prefs=new BMenuItem(T_MENU_GENERAL_PREFS,new BMessage(MENU_PREFS)));
	edit_track->AddItem(new BMenuItem(T_MENU_REMOVE,new BMessage(REMOVE_TRACK),'R'));	
	edit_track->AddSeparatorItem();
	edit_track->AddItem(copy=new BMenuItem(T_MENU_CUT,new BMessage(MENU_CUT),'X'));
	edit_track->AddItem(cut=new BMenuItem(T_MENU_COPY,new BMessage(MENU_COPY),'C'));
	edit_track->AddItem(paste=new BMenuItem(T_MENU_PASTE,new BMessage(MENU_PASTE),'V'));
	edit_track->AddSeparatorItem();
	edit_track->AddItem(rename=new BMenuItem(T_GEN_RENAME,new BMessage(MENU_RENAME)));

	edit_mea->AddItem(mea_copy=new BMenuItem(T_MENU_CUT,new BMessage(MENU_MEA_CUT),'X',B_SHIFT_KEY));
	edit_mea->AddItem(mea_cut=new BMenuItem(T_MENU_COPY,new BMessage(MENU_MEA_COPY),'C',B_SHIFT_KEY));
	edit_mea->AddItem(mea_paste=new BMenuItem(T_MENU_PASTE,new BMessage(MENU_MEA_PASTE),'V',B_SHIFT_KEY));
	
	
	paste->SetEnabled(false);
	mea_paste->SetEnabled(false);
	
	edit_track->SetTargetForItems(be_app);
	edit_mea->SetTargetForItems(be_app);

	AddChild(menuBar);
}


void
MainWindow::UpdateRecents(){	
}

void		
MainWindow::PlayButtonOn(bool state){
	if(Lock()){
	 fPanel->PlayButtonOn(state);
	 Unlock();
	}
}

void MainWindow::Close(){}

MainWindow::~MainWindow(){
		SaveConfig();				
}


bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return false;	
}
void
MainWindow::Saved()
{
	entry_ref ref;
	curSong->getEntry()->GetRef(&ref);
	
	SetTitle(ref.name);
	//save_item->SetEnabled(false);
		
}
void
MainWindow::Init()
{

	fTracksPanel->Init(ticks);
	
	
	if(Lock())
	{
		BMenu* x=TrackManager::Get()->getMenu();
		x->SetTargetForItems(this);
		menuBar->AddItem(x);
		/***/
		// recent documents
		//BMessage treno;
		//be_roster->GetRecentDocuments(&treno,10,NULL,"application/x-vnd.xeD.XRS");
		//treno.PrintToStream();
		//FIX ME (up)
		Unlock();
	}

}

void
MainWindow::Reset(Song* s,bool juicereset)
{
		fPanel->Reset(s,fTracksPanel);
		fTracksPanel->Reset(s);

		if(s->getEntry()!=NULL) {
					
				entry_ref t;
				s->getEntry()->GetRef(&t);
				SetTitle(t.name);
		} else
		
		SetTitle(DEFAULT_TITLE);
			
		
		ticks->SetNumberNotes(s->getNumberNotes());
		curSong=s;
		
		PlayButtonOn(JuiceEngine::Get()->IsPlaying());
		
}
void
MainWindow::WindowActivated(bool active)
{
	if(!fPanel) return;
	
	if(active)
		//fPanel->SetEventMask(B_KEYBOARD_EVENTS);
		fPanel->MakeFocus(true);
	else
		//fPanel->SetEventMask(0);
		fPanel->MakeFocus(false);	
	
}

void
MainWindow::SetWheelTarget(BHandler *h)
{
	if(h)
		wheel_target=h;
	else
		wheel_target=NULL; //tp;
}

void
MainWindow::MessageReceived(BMessage* message)
{
	entry_ref ref;
	status_t err;
	
	
	switch(message->what)
	{
	
		case 'hide':
			WindowManager::Get()->CloseAll();
			break;
		
		case MOUSE_WHEEL: /* mouse wheel */
			if(wheel_target) wheel_target->Looper()->PostMessage(message,wheel_target);
			break;
		
		case 'obsv':
			//fix me!
			//printf("here\n");
			PostMessage(message,fPanel);
			break;
		case MENU_SETTINGS:
			Panels::showSettings(curSong);		
			break;
		case MENU_PREFS:
			Panels::showPrefsPanel();
			break;
		case MENU_EXPORT:
			Panels::showExport(MeasureManager::Get()->GetCurrentPattern()+1,curSong->getNumberMeasure(), NULL);
			break;
		
		case PLAY_START:
			JuiceEngine::Get()->Start();
			break;
		case PLAY_ALL_SET:
			MeasureManager::Get()->SetPatternMode(fPanel->isAllPat());
			break;
		/*case TEMPO_MOV:
			JuiceEngine::Get()->PostMessage(message);
			break;*/
		case TRACK_SET:
			PostMessage(message, fTracksPanel);
			break;
		case MENU_COPY:
		case MENU_CUT:
			paste->SetEnabled(true);
		case MENU_PASTE:
			fTracksPanel->resetPattern();
			break;
		case MENU_RENAME:
			fTracksPanel->RenameSelectedTrack();
			break;
		case MENU_MEA_COPY:
		case MENU_MEA_CUT:
			mea_paste->SetEnabled(true);
		case MENU_MEA_PASTE:
			fTracksPanel->resetPattern();
			break;
		
		case ADD_TRACK:		
			AddTrack(TrackManager::Get()->getTrack(message->FindInt16("id")));
			break;
		case REMOVE_TRACK:
			
			if(fTracksPanel->getNumberJTrack()<=0) return ;
			
			if(AskRemove(T_DIALOG_REMOVE_TRACK)==B_OK){
			
				Track* trk=fTracksPanel->getCurrentTrack();
				int pos=curSong->getIndexOfTrack(trk);
				XrsMidiIn::Get()->UnregisterCh(trk,trk->GetMidiInCh());
				XHost::Get()->SendMessage(X_LockSem,0);
				XHost::Get()->AllowLock(false);
					curSong->RemoveTrack(trk);
				XHost::Get()->AllowLock(true);
				XHost::Get()->SendMessage(X_UnLockSem,0);
				fTracksPanel->RemoveTrackAt(pos);
				fTracksPanel->resetPattern(); //UI Refresh!
			   }
			  
			break;
			
		case EDIT_MSG:
			WindowManager::Get()->Switch(MatrixWindow::Get());		
			break;
		case 'VST':
			WindowManager::Get()->Switch(MixerWindow::Get());
			break;
		case ADD_PATTERN:
			{
			int z = MeasureManager::Get()->GetCurrentPattern();
			XHost::Get()->SendMessage(X_LockSem,0);
				curSong->AddMeasure();
			XHost::Get()->SendMessage(X_UnLockSem,0);
			fPanel->ResetMeasureCount();
			MatrixWindow::Get()->Reset(curSong->getSequence());
			MeasureManager::Get()->SetCurrentPattern(z);
			}
			break;
		
		case REMOVE_PATTERN:
		{
			int f=MeasureManager::Get()->GetCurrentPattern();
			
			XHost::Get()->SendMessage(X_LockSem,0);
				err=curSong->RemoveMeasure(f);
			XHost::Get()->SendMessage(X_UnLockSem,0);
			
			
			if(err==B_OK){
			
				fPanel->ResetMeasureCount();
				MatrixWindow::Get()->Reset(curSong->getSequence());
			
			if( f >= curSong->getNumberMeasure() ) 
				
					f=curSong->getNumberMeasure()-1;
				
			 MeasureManager::Get()->SetCurrentPattern(f);

			
			}
			
			
		}
		break;	
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

status_t
MainWindow::AskRemove(const char *txt)
{
	BAlert *sam;
	int32 sam_say;
	
	BString ask(T_GEN_REMOVE);
	ask << " ";
	
	sam=new BAlert(ask.String(),txt,T_GEN_REMOVE,T_GEN_NO);
	sam_say=sam->Go();
	
	if(sam_say==0) return B_OK;
	else return B_ERROR;
}
void
MainWindow::AddTrack(Track* trk)
{
	if (trk==NULL) return;
	
	trk->Message(SystemReset,0);
	trk->Message(TempoChange,(float)curSong->getNoteSize());
		XHost::Get()->SendMessage(X_LockSem,0);
			curSong->AddTrack(trk);
		XHost::Get()->SendMessage(X_UnLockSem,0);
	fTracksPanel->AddTrack(curSong->getNumberTrack()-1);
}

