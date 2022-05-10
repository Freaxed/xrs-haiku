/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
#include 	"TickView.h"
#include 	"XHost.h"
#include	"MixerWindow.h"
#include 	"TrackManager.h"
#include	"MeasureManager.h"
#include	"locale.h"

#ifdef XRS_MIDI
	#include 	"XrsMidiIn.h"
#endif
#include 	"version.h"
#include	"Song.h"
#include	"WindowManager.h"

#include	<Roster.h>
#include 	<stdio.h>
#include 	<Application.h>
#include 	<Path.h>
#include 	<Entry.h>
#include 	<Directory.h>
#include    "SplitPane.h"
#include	"PlaylistBox.h"
#include	"TrackInfoBox.h"
#include	<ControlLook.h>

MainWindow*
MainWindow::Get()
{
	static	MainWindow*	instance = NULL;
	if (instance == NULL) 
			instance = new MainWindow();
	return instance;
}


MainWindow::MainWindow() :
	XrsWindow(BRect(WINDOW_X_POS,WINDOW_Y_POS,WINDOW_X_POS+WINDOW_XL,WINDOW_Y_POS+WINDOW_YL),"XRS", B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE)//|B_WILL_ACCEPT_FIRST_CLICK) //|B_AVOID_FOCUS)

{
	SetName("");
	
	CreateMenu();
	float menuY, menuX;
	menuBar->GetPreferredSize(&menuX,&menuY);
	BRect nb(Bounds());
	
	nb.top    = menuY + 1;
	nb.bottom = nb.top + XPANEL_H - 1;
	
	
	fPanel = new XPanel(nb);
	AddChild(fPanel);

	menuY = nb.bottom;
	

	
	nb = Bounds();	
	nb.top     = menuY + 1;

	BBox*	background = new BBox(nb, "background", B_FOLLOW_ALL);
	BBox*	tracksBackground = new BBox(BRect(0, 0, nb.right, (nb.Height() / 2) - B_H_SCROLL_BAR_HEIGHT ), "tracksBackground", B_FOLLOW_ALL);

	BRect local = tracksBackground->Bounds();
	
	local.right  -= 182 + B_V_SCROLL_BAR_WIDTH;
	local.bottom -= B_H_SCROLL_BAR_HEIGHT + 1;

	ticks = new TickView(BRect(0, 1, local.right, INFO_BAR_LY + 3));
	tracksBackground->AddChild(ticks);

	local.top     = INFO_BAR_LY + 4;

	fTracksPanel = new TracksPanel(local);

	
	
	BScrollView		*scroll_view;
	scroll_view = new BScrollView("XRScrollView", fTracksPanel , B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS, true, true, B_FANCY_BORDER);
	tracksBackground->AddChild(scroll_view);

	tracksBackground->AddChild((fTrackInfoBox = new TrackInfoBox(BRect(local.right + B_V_SCROLL_BAR_WIDTH + 2, 0, nb.right,  280))));
	
	fPlaylistBox = new PlaylistBox(local);		
	SplitPane*	splitPane = new SplitPane(background->Bounds(), tracksBackground, fPlaylistBox, B_FOLLOW_ALL);
	splitPane->SetAlignment(B_HORIZONTAL);
	background->AddChild(splitPane);
	splitPane->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	splitPane->SetResizeViewOne(true, true);
	AddChild(background);

	LoadConfig();

	float a,b,c,d;
	GetSizeLimits(&a,&b,&c,&d);
	SetSizeLimits(300, b, 300, d);
	
	SetWheelTarget(NULL);
	
	AddShortcut('-', 0, new BMessage('hide'));
}

void
MainWindow::CreateMenu(){

	BMenu* menuEdit;
	BMenu* edit_track;
	BMenu* edit_mea;
	BMenuItem*	edit_prefs;	
	BMenuItem*	quit_item;
	
	menuBar = new BMenuBar(BRect(0,0,0,0), "Full Menu Bar");

	menuBar->AddItem(menuFile  = new BMenu(T_MENU_FILE));
	menuBar->AddItem(menuEdit  = new BMenu(T_MENU_EDIT));
	menuFile->AddItem(new_item = new BMenuItem("New",new BMessage(MENU_NEW_EMPTY),'N'));
	menuFile->AddItem(open_item = new BMenuItem(T_MENU_OPEN, new BMessage(MENU_OPEN),     'O'));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(save_item   = new BMenuItem(T_MENU_SAVE,new BMessage(MENU_SAVE),'S'));
	menuFile->AddItem(saveas_item = new BMenuItem(T_MENU_SAVE_AS,new BMessage(MENU_SAVEAS)));
	menuFile->AddItem(export_item = new BMenuItem(T_MENU_EXPORT_AS_WAVE,new BMessage(MENU_EXPORT)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(new BMenuItem(T_MENU_ABOUT,new BMessage(B_ABOUT_REQUESTED)));
	menuFile->AddSeparatorItem();
	menuFile->AddItem(quit_item=new BMenuItem(T_MENU_QUIT,new BMessage(B_QUIT_REQUESTED),'Q'));	//Un po' brutale..
	menuFile->SetEnabled(true);

	menuFile->SetTargetForItems(be_app);
	quit_item->SetTarget(this);

	menuEdit->AddItem(edit_track=new BMenu(T_MENU_TRACK));
	menuEdit->AddItem(edit_mea=new BMenu(T_MENU_MEASURE));
	menuEdit->AddItem(new BMenuItem(T_MENU_PLAYLIST,new BMessage(EDIT_MSG)));
	menuEdit->AddSeparatorItem();
	menuEdit->AddItem(settings_item = new BMenuItem(T_MENU_SONG_SETTINGS,new BMessage(MENU_SETTINGS)));
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
MainWindow::PlayButtonOn(bool state)
{
	if(Lock()){
	 fPanel->OnPlay(state);
	 Unlock();
	}
}

	

MainWindow::~MainWindow()
{
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
	ResetTitle();		
}
void
MainWindow::Init()
{
	fTracksPanel->Init(ticks, fTrackInfoBox);

	if(Lock())
	{
		BMenu* x = TrackManager::Get()->getMenu();
		x->SetTargetForItems(this);
		menuBar->AddItem(x);
		Unlock();
	}
}

void
MainWindow::ResetTitle()
{
	BString title(DEFAULT_TITLE);
	
	if(curSong && curSong->getEntry() != NULL)
	{
		entry_ref t;
		curSong->getEntry()->GetRef(&t);
		title += " - ";
		title += t.name;
	} 
	
	SetTitle(title.String());
	LogTrace("MainWindow, title set to [%s]", title.String());
}

void
MainWindow::ResetToSong(Song* s)
{
	LogTrace("Resetting MainWindow to new Song...");
	curSong = s;

	//fPanel->ResetToSong(curSong, fTracksPanel);
	fTracksPanel->ResetToSong(curSong);
	fPanel->ResetToSong(curSong, fTracksPanel);
	fPlaylistBox->Reset(curSong->getSequence(), curSong->getNumberNotes());
	
	ResetTitle();	
	
	ticks->SetNumberNotes(curSong->getNumberNotes());		
	PlayButtonOn(JuiceEngine::Get()->IsPlaying());		
}
void
MainWindow::WindowActivated(bool active)
{
	if(fPanel) 
		fPanel->MakeFocus(active);
}

void
MainWindow::SetWheelTarget(BHandler *h)
{
	wheel_target = h;
}

void		
MainWindow::OnPlay(bool doPlay) 
{	
	if (Lock()) {
		
		doPlay ? JuiceEngine::Get()->Start() : JuiceEngine::Get()->Stop();
		
		doPlay = JuiceEngine::Get()->IsPlaying(); //could change (in case you play an already playing engine, it will stop..)
		
	      saveas_item->SetEnabled(!doPlay);
		    save_item->SetEnabled(!doPlay);
		     new_item->SetEnabled(!doPlay);
	        open_item->SetEnabled(!doPlay);	
		  export_item->SetEnabled(!doPlay);	
		settings_item->SetEnabled(!doPlay);	
		
		PlayButtonOn(doPlay);
	
	Unlock();
	}	
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
		case B_MOUSE_WHEEL_CHANGED:
			if(wheel_target) wheel_target->Looper()->PostMessage(message,wheel_target);
			break;
		case MENU_SETTINGS:
			Panels::showSettings(curSong);		
			break;
		case MENU_PREFS:
			Panels::showPrefsPanel();
			break;
		case MENU_EXPORT:
			Panels::showExport(MeasureManager::Get()->GetCurrentPattern()+1,curSong->getNumberMeasure());
			break;		
		case PLAY_START:
			OnPlay (message->GetInt32("be:value", 1) == 1);
			break;
		case PLAY_ALL_SET:
			MeasureManager::Get()->SetPatternMode(fPanel->isAllPat());
			break;
		case TRACK_SET:
			PostMessage(message, fTracksPanel);
			break;
		case MENU_COPY:
		case MENU_CUT:
			paste->SetEnabled(true);
		case MENU_PASTE:
			fTracksPanel->resetPattern(MeasureManager::Get()->GetCurrentPattern());
			break;
		case MENU_RENAME:
			fTracksPanel->RenameSelectedTrack();
			break;
		case MENU_MEA_COPY:
		case MENU_MEA_CUT:
			mea_paste->SetEnabled(true);
		case MENU_MEA_PASTE:
			fTracksPanel->resetPattern(MeasureManager::Get()->GetCurrentPattern());
			break;
		
		case ADD_TRACK:		
			AddTrack(TrackManager::Get()->getTrack(message->FindInt16("id")));
			break;
		case REMOVE_TRACK:
			
			if(fTracksPanel->getNumberJTrack()<=0) return ;
			
			if(AskRemove(T_DIALOG_REMOVE_TRACK)==B_OK){
			
				Track* trk=fTracksPanel->getCurrentTrack();
				int pos=curSong->getIndexOfTrack(trk);
				#ifdef XRS_MIDI
					XrsMidiIn::Get()->UnregisterCh(trk,trk->GetMidiInCh());
				#endif
				XHost::Get()->SendMessage(X_LockSem,0);
					curSong->RemoveTrack(trk);
				XHost::Get()->SendMessage(X_UnLockSem,0);
				fTracksPanel->RemoveTrackAt(pos);
				fTracksPanel->resetPattern(MeasureManager::Get()->GetCurrentPattern()); //UI Refresh!
			   }
			  
			break;
			
		case EDIT_MSG:
			LogInfo("Button to be removed!");	
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
			fPlaylistBox->Reset(curSong->getSequence(), curSong->getNumberNotes());
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
				fPlaylistBox->Reset(curSong->getSequence(), curSong->getNumberNotes());
			
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
	return (new BAlert(T_GEN_REMOVE, txt, T_GEN_REMOVE, T_GEN_NO))->Go() == 0 ? B_OK : B_ERROR;
}
void
MainWindow::AddTrack(Track* trk)
{
	if (!trk) 
		return;
	
	JuiceEngine::Get()->LockEngine("AddTrack");
	JuiceEngine::Get()->OnNewTrack(trk);
	curSong->AddTrack(trk);
	JuiceEngine::Get()->UnlockEngine("AddTrack");
	
	fTracksPanel->AddTrack(curSong->getNumberTrack() - 1);
}

