/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 *
 */

#include "TheApp.h"

#include <Alert.h>
#include <Font.h>
#include <TextView.h>
#include <FilePanel.h>

#include 	"GfxMsg.h"
#include 	"JFileManager.h"
#include 	"Configurator.h"
#include 	"Pattern.h"
#include 	"Track.h"
#include 	"Note.h"
#include 	"MeasureManager.h"
#include 	"Xed_Utils.h"
#include 	"AboutBox.h"

#include 	"TrackManager.h"
#include 	"MixerWindow.h"
#include	"VstManager.h"
#include	"XHost.h"


#include	"ValuableManager.h"

#include	"WindowManager.h"
#include	"MatrixWindow.h"
#include	"XrsMidiIn.h"
#include	 "XrsMidiOut.h"
#include    "MBWindow.h"
#include	"TrackInfoWindow.h"
#include 	"PotViewer.h"
#include 	"PMixer.h"
#include	"JuiceEngine.h"
#include	"BasicModelManager.h"


extern PotViewer* potviewer = NULL;

#define SCREEN_TIME 2000000

TheApp::TheApp(const char * signature) :
	BApplication(signature)
{
	currentSong=NULL;
	clipboard=new Pattern(NUM_NOTES);
	dinamic_clip=new BList(1);
	PrepareToRun();
}
TheApp::~TheApp()
{
	//if(msucco->Lock()){
	if(msucco->Acquire("~TheApp") == B_OK){	
		msucco->Stop();
		msucco->ReallyStop();	//stop the engine	
		xhost->AllowLock(false);
		 
		 //msucco->Acquire("QuitRequest");
		 
		 if(trackinfo->Lock())				
			 trackinfo->Quit();
		
		 track_manager->Close();
			 		
		 jfm->CloseSong(currentSong);
		
		 
			 
		 if(mixerWin->Lock())
		 {
			if(mixerWin->IsHidden())
		 		Config()->PutFloatKey("mixer_window_show",0);
		 	else
		 		Config()->PutFloatKey("mixer_window_show",1);
		 		
			mixerWin->Quit();
		 }
		 if(main_window->Lock()) 
		 	main_window->Quit();
		 if(media_browser->Lock())
		 {
		 	if(media_browser->IsHidden())
		 		Config()->PutFloatKey("media_browser_show",0);
		 	else
		 		Config()->PutFloatKey("media_browser_show",1);
		 		
		 	media_browser->Quit();
		 }
		 if(mw->Lock()) 
		 	mw->Quit();
		 	
		 fVManager->DumpValues();
		 	
		 delete fModel;
	     
	     if(fVManager->Lock())
			fVManager->Quit();
			
		XrsMidiIn::Get()->Release();
		XrsMidiOut::Get()->Release();
		
		if(msucco->Lock())			
			msucco->Quit();
	
	}
	
	if(potviewer && potviewer->Lock()){
	
		potviewer->Quit();
	}
	
	delete a_mixer;

	
	delete track_manager;
	delete vst_manager;
	
	
	
	Configurator::Get()->cf_Close(); //Configuration/Preferences		
	printf("Quit\n");
}
void
TheApp::PrepareToRun()
{


		
	AboutBox *ab=new AboutBox(false);
	bigtime_t start=system_time();
	ab->Show();
	
	
	
	Configurator::Get()->cf_Init("XRSConfig");	
	
	ab->setText("loading..vst");
	vst_manager=VstManager::Get();
	
	ab->setText("loading..Extensions");
	
	win_manager=WindowManager::Get();
	AddCommonFilter(win_manager);
	
	fVManager = ValuableManager::Get();
	
	fModel = new BasicModelManager();
	
	mea_manager= MeasureManager::Get();
	AddCommonFilter(mea_manager);
	
	trackinfo= TrackInfoWindow::Get();

	
	track_manager= TrackManager::Get();
	track_manager->Init();
	 	
	jfm= JFileManager::Get();
	jfm->Init();
	
		
	ab->setText("loading..Mixer");
	
	a_mixer= PMixer::Get();
	
	mixerWin= MixerWindow::Get();
	
	
	
	//matrix
	mw= MatrixWindow::Get();
	mw->Show();
	mw->Hide();
	
	xhost=XHost::Get();	
	//juice
	msucco= JuiceEngine::Get();
	main_window= MainWindow::Get();

	
	msucco->Init();
	
	main_window->Init();
	
	bigtime_t stop=system_time();
	if(stop-start<SCREEN_TIME)
		snooze(SCREEN_TIME-(stop-start));
		
	if(ab->Lock()) ab->Quit();	
	
	float show;
	media_browser=new MBWindow();
	win_manager->Show(media_browser);
	
	show=Config()->FloatKey("media_browser_show",1);
	
	if(show<1)
		win_manager->Hide(media_browser);
	//
	show=Config()->FloatKey("mixer_window_show",0);
	win_manager->Show(mixerWin);
	
	if(show<1)
		win_manager->Hide(mixerWin);
	
	main_window->AddToSubset(mw);
	main_window->Show();
	
//	obs_volumes->setTarget("master",(Pannable*)my_mixer->getLine(0));
//	obs_volumes->Run();

	
	
		
	
	
	
	DefaultSong(false);
	
	if(currentSong){
		delete currentSong->getEntry();
		currentSong->setEntry(NULL);
	}
}
	


void 
TheApp::LoadSong(entry_ref ref){

	if(msucco->Acquire("LoadSong") == B_OK)
	{
		
		msucco->Stop();			  //no more sound processing
		xhost->AllowLock(false);  // nobody can lock/unlock juice
		 
		LoadRequest(ref);
			
		mea_manager->Reset(currentSong->getSequence());
			
		if (main_window->Lock())
		{
			main_window->Reset(currentSong);
			main_window->Unlock();
		}
	
		if (mw->Lock())
		{
			mw->Reset(currentSong->getSequence());	
			mw->Unlock();
		}
	
		//fix me
		mixerWin->LoadSettings(currentSong->mixer_settings);
			
		jfm->AnalizeError(currentSong->mixer_settings);
			
		XrsMidiIn::Get()->Reset(currentSong);
		
		msucco->ResetSong(currentSong);
			
		xhost->AllowLock(true);
		msucco->Release("LoadSong");
		
		Panels::showErrors(jfm->ErrorsLog());
	}
	
	// Show errors!;
	
	if(currentSong && currentSong->getSizeDescription()>0 && currentSong->popupdesc==true)
			(new BAlert("XRS ",currentSong->getDescription(), "Ok",NULL,NULL,B_WIDTH_AS_USUAL,B_EMPTY_ALERT))->Go(NULL);
	
	
}
void
TheApp::EmptySong()
{
	status_t	ask;
	ask=jfm->AskForClose(currentSong);
		
	if(ask==B_OK)
	{
		entry_ref ref;
		LoadSong(ref);
	}
}
void
TheApp::DefaultSong(bool close=true)
{
	entry_ref ref;
	status_t	ask=B_ERROR;
	
	if(close) ask=jfm->AskForClose(currentSong);
	
		
	if(!close || ask==B_OK){
			
		entry_ref ref;
		XUtils::GetDefaultSongRef(&ref);
		LoadSong(ref);
	
	}
}
void
TheApp::MessageReceived(BMessage* message)
{
	entry_ref ref;
	
	status_t	ask;
	//int id;
	//int32 old;
	
	switch(message->what)
	{
	
	case MENU_NEW_EMPTY:
	
		EmptySong();
	
	break;
		
	case MENU_NEW_DEFAULT:
	
		DefaultSong();
		
		break;
		
	case MENU_SAVE:
		
		
		jfm->Save(currentSong,false);
	
		break;
	case MENU_SAVEAS:
		
		
		jfm->Save(currentSong,true);
	
		break;
	case	X_REFS_RECEIVED:
		OpenRefs(message);
		break;	
	case MENU_OPEN:
		ask=jfm->AskForClose(currentSong);
		if(ask==B_OK) jfm->Load();
		break;
	case MENU_EXPORT:
		main_window->PostMessage(message);
	break;
	
	case MENU_ZIP:
		jfm->Zip(currentSong);
	break;
	
	case 'zipo':
		if(message->FindRef("directory",&ref)==B_OK)
			jfm->DoZip(ref,message->FindString("name"),currentSong);
	break;			
	case B_SAVE_REQUESTED:
		if(message->FindRef("directory",&ref)==B_OK)
		{
			jfm->SaveReq(ref,message->FindString("name"),currentSong);
			main_window->Saved();
		}
	break;
	case	'expt':	// EXPORT AS WAVE

			jfm->RenderAsWave(message);

	break;
	
	case MENU_COPY:
		if(main_window->Lock())
		{	
		
			clipboard->setNumberNotes(currentSong->getNumberNotes());
			CopyPattern((track_manager->getCurrentTrack())->getPatternAt(mea_manager->GetCurrentPattern()),clipboard);
			main_window->Unlock();
		}
		main_window->PostMessage(message);
	break;
	case MENU_CUT:
		
		if(main_window->Lock())
		{
			clipboard->setNumberNotes(currentSong->getNumberNotes());
			CopyPattern((track_manager->getCurrentTrack())->getPatternAt(mea_manager->GetCurrentPattern()),clipboard);
			ClearPattern((track_manager->getCurrentTrack())->getPatternAt(mea_manager->GetCurrentPattern()));
			main_window->Unlock();
		}
		main_window->PostMessage(message);
	break;
	case MENU_PASTE:
		
		if(main_window->Lock())
		{
			clipboard->setNumberNotes(currentSong->getNumberNotes());
			CopyPattern(clipboard,(track_manager->getCurrentTrack())->getPatternAt(mea_manager->GetCurrentPattern()));
			main_window->Unlock();
		}
		main_window->PostMessage(message);
	break;
	case MENU_MEA_COPY:
		
		if(main_window->Lock())
		{
			for(int i=0;i<dinamic_clip->CountItems();i++)
			{
				delete dinamic_clip->ItemAt(i);	
			}
			
			dinamic_clip->MakeEmpty();
						
			for(int i=0;i<currentSong->getNumberTrack();i++)
			{
				Pattern *r=new Pattern(currentSong->getNumberNotes());
				r->setNumberNotes(currentSong->getNumberNotes());
				dinamic_clip->AddItem(r);
				CopyPattern(currentSong->getTrackAt(i)->getPatternAt(mea_manager->GetCurrentPattern()),r);
											
			}
						
			main_window->Unlock();
		}
		main_window->PostMessage(message);
	break;
	case MENU_MEA_CUT:
		
		if(main_window->Lock())
		{
			for(int i=0;i<dinamic_clip->CountItems();i++)
			{
				delete dinamic_clip->ItemAt(i);	
			}
			
			dinamic_clip->MakeEmpty();
						
			for(int i=0;i<currentSong->getNumberTrack();i++)
			{
				Pattern *r=new Pattern(currentSong->getNumberNotes());
				r->setNumberNotes(currentSong->getNumberNotes());
				dinamic_clip->AddItem(r);
				CopyPattern(currentSong->getTrackAt(i)->getPatternAt(mea_manager->GetCurrentPattern()),r);
											
			}
			for(int i=0;i<currentSong->getNumberTrack();i++)
			{
				ClearPattern(currentSong->getTrackAt(i)->getPatternAt(mea_manager->GetCurrentPattern()));
			}
			main_window->Unlock();
			
		}
		main_window->PostMessage(message);
	break;
	case MENU_MEA_PASTE:
		
		if(dinamic_clip->CountItems() > 0) 
		{
			 if(main_window->Lock())
	 		{
	 			
				for(int i=0;i<dinamic_clip->CountItems();i++)
				{
					if(i<currentSong->getNumberTrack())
					{
						Pattern *r=(Pattern*)dinamic_clip->ItemAt(i);
						r->setNumberNotes(currentSong->getNumberNotes());
						CopyPattern(r,currentSong->getTrackAt(i)->getPatternAt(mea_manager->GetCurrentPattern()));
					}
				}
				main_window->Unlock();
			}
			main_window->PostMessage(message);
		}
	break;
	case SONG_RESET:
		if(main_window->Lock()){
		main_window->Reset(currentSong,false);
		main_window->Unlock();
		}
		if(mw->Lock()){
		mw->Reset(currentSong->getSequence());
		mw->Unlock();
		}
	break;
	case MENU_RENAME:
		main_window->PostMessage(message);
	break;
	case ADD_TN306:
	case ADD_TRACK:
	case REMOVE_TRACK:
		main_window->PostMessage(message);
	break;
	default:
	 BApplication::MessageReceived(message);
	break;
	}
}	
void
TheApp::CopyPattern(Pattern* from,Pattern* to)
{
	for(int i=0;i<from->getNumberNotes();i++)
	{
		to->getNoteAt(i)->setValue(from->getNoteAt(i)->getValue());
		to->getNoteAt(i)->setNote(from->getNoteAt(i)->getNote());
		to->getNoteAt(i)->setOct(from->getNoteAt(i)->getOct());
		to->getNoteAt(i)->setVolume(from->getNoteAt(i)->getVolume());	
	}
}
void
TheApp::ClearPattern(Pattern* to)
{
	for(int i=0;i<to->getNumberNotes();i++)
	{
		to->getNoteAt(i)->setValue(false);	
		to->getNoteAt(i)->setNote(0);	
		to->getNoteAt(i)->setOct(0);	
		to->getNoteAt(i)->setVolume(0.8);	
	}
}
bool
TheApp::QuitRequested()
{
	status_t	ask;
	ask=jfm->AskForClose(currentSong);
		
	return (ask==B_OK);
	
	
}
void
TheApp::RefsReceived(BMessage *message)
{
	entry_ref ref;
	
	if(!be_app->IsLaunching())
	{
		status_t ask=jfm->AskForClose(currentSong);
		if(ask!=B_OK) return;	
	}
	if(message->FindRef("refs",&ref)==B_OK)
		{			
			LoadSong(ref);
			
			
			
		}
	
}
void
TheApp::OpenRefs(BMessage *message)
{
	entry_ref ref;
				
	if(message->FindRef("refs",&ref)==B_OK)
			LoadSong(ref);
	
}

status_t
TheApp::LoadRequest(entry_ref ref)
{
	Song* new_song;
	new_song=jfm->EmptySong();
	
	
	CloseSong(currentSong);
	jfm->LoadFile(ref,new_song);
	
	currentSong=new_song;
	return B_OK;
}
void
TheApp::CloseSong(Song *s)
{
	/* Clearing Mixer & Other stuff ?*/
	jfm->CloseSong(s);
	track_manager->Restart();
}
void
TheApp::AboutRequested()
{
	
	AboutBox *ab=new AboutBox(true);
	ab->Show();
		
	printf("Written by Anzani Andrea\nSend comments to : oxygenum@tiscali.it\nFreeware\n");
}
//-
