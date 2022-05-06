/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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

#include	"ValuableManager.h"

#include	"WindowManager.h"
#ifdef XRS_MIDI
	#include	"XrsMidiIn.h"
	#include	"XrsMidiOut.h"
#endif
#ifdef MEDIA_BROWSER
	#include    "MBWindow.h"
#endif
#include	"TrackInfoWindow.h"
#include 	"PotViewer.h"
#include 	"PMixer.h"
#include	"JuiceEngine.h"
#include	"BasicModelManager.h"
#include	"ValuableMonitorWindow.h"


#define SCREEN_TIME 0 //2000000

TheApp::TheApp(const char * signature) :
	BApplication(signature)
{
	currentSong=NULL;
	clipboard=new Pattern(NUM_NOTES); //FIXME: the numnotes may vary!
	dinamic_clip=new BList(1);
	PrepareToRun();
}

TheApp::~TheApp()
{
	msucco->Stop();
	if(msucco->LockEngine("~TheApp")){

		msucco->ReallyStop();	//stop the engine	
		 
		if(fValuableMonitor && fValuableMonitor->Lock()) {
			fValuableMonitor->Quit();
		}
		 	
		delete fModel;
	     
	    if(fVManager->Lock())
			fVManager->Quit();
		 
		if(TrackInfoWindow::Get()->Lock())				
			 TrackInfoWindow::Get()->Quit();
		
		track_manager->Close();
			 		
		delete (currentSong);
		 
			 
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

#ifdef MEDIA_BROWSER		 	

		if(media_browser->Lock())
		{
		 	if(media_browser->IsHidden())
		 		Config()->PutFloatKey("media_browser_show",0);
		 	else
		 		Config()->PutFloatKey("media_browser_show",1);
		 		
		 	media_browser->Quit();
		}
#endif 
		 	

#ifdef XRS_MIDI
		XrsMidiIn::Get()->Release();
		XrsMidiOut::Get()->Release();
#endif		
		msucco->Quit();
	
	}
	
	if(PotViewer::Get()->Lock()){	
		PotViewer::Get()->Quit();
	}
	
	delete PMixer::Get();	
	delete track_manager;
	delete VstManager::Get();
	
	Configurator::Get()->Close();
	LogInfo("Goodbye XRS!");
}
void
TheApp::PrepareToRun()
{
	AboutBox *ab = new AboutBox(false);
	bigtime_t start = system_time();
	ab->Show();

	Configurator::Get()->Init("XRSConfig");	
	
	ab->setText("loading..vst");
	VstManager::Get();
	
	ab->setText("loading..Extensions");
	
	win_manager = WindowManager::Get();
	AddCommonFilter(win_manager);
	
	fVManager = ValuableManager::Get();
	
	fModel = new BasicModelManager();
	
	mea_manager = MeasureManager::Get();
	
	TrackInfoWindow::Get();

	
	track_manager = TrackManager::Get();
	track_manager->Init();
	 	
	jfm = JFileManager::Get();
	jfm->Init();
	
		
	ab->setText("loading..Mixer");
	

	mixerWin = MixerWindow::Get();
	

	//juice
	msucco = JuiceEngine::Get();
	main_window = MainWindow::Get();

	
	msucco->Init();
	
	main_window->Init();
	
	bigtime_t stop=system_time();
	if(stop-start<SCREEN_TIME)
		snooze(SCREEN_TIME-(stop-start));
		
	if(ab->Lock()) ab->Quit();	
	
	float show;
	
#ifdef MEDIA_BROWSER

	media_browser = new MBWindow();
	win_manager->Show(media_browser);
	
	show=Config()->FloatKey("media_browser_show",1);
	
	if(show<1)
		win_manager->Hide(media_browser);
#endif
	//
	show=Config()->FloatKey("mixer_window_show",0);
	win_manager->Show(mixerWin);
	
	if(show<1)
		win_manager->Hide(mixerWin);
	
	main_window->Show();
	
	fValuableMonitor = NULL;
	if (Logger::IsDebugEnabled()) {
		fValuableMonitor = new ValuableMonitorWindow();
		fValuableMonitor->Show();
		fVManager->AttachMonitorValuableManager(fValuableMonitor);
	}

	EmptySong(false);
}
	


void 
TheApp::LoadSong(entry_ref ref)
{
	BAlert* idle = XUtils::ShowIdleAlert("Loading song..");
	
	msucco->Stop();			  // no more sound processing?

	fModel->ResetValues();	 // default valuables (mixer & tempo!)
	
	Song* oldSong = LoadRequest(ref); 		 // triggers the new valuables! (mixer, NOT TEMPO!?!?!)
		
	mea_manager->Reset(currentSong->getSequence());
		
	if (main_window->Lock())
	{
		main_window->ResetToSong(currentSong);	//Update UI according to struct(song);
		main_window->Unlock();
	}

	
	if (MixerWindow::Get()->Lock())
	{
		MixerWindow::Get()->ResetUI();
		MixerWindow::Get()->Unlock();
	}			

#ifdef XRS_MIDI	
	XrsMidiIn::Get()->Reset(currentSong);		
#endif

	msucco->LockEngine("ResetSong");
	msucco->ResetSong(currentSong);
	msucco->UnlockEngine("ResetSong");

	delete oldSong;
	
	XUtils::HideIdleAlert(idle);	
	
	Panels::showLoadingError();

	if(currentSong && currentSong->getSizeDescription()>0 && currentSong->popupdesc==true)
			(new BAlert("XRS ",currentSong->getDescription(), "Ok",NULL,NULL,B_WIDTH_AS_USUAL,B_EMPTY_ALERT))->Go(NULL);
}
void
TheApp::EmptySong(bool close = true)
{
	status_t	ask = B_OK;
	if(close)
		ask = jfm->AskForClose(currentSong);

	if(ask == B_OK)
	{
		entry_ref ref;
		LoadSong(ref);
	}
}

void
TheApp::MessageReceived(BMessage* message)
{
	entry_ref   ref;

	switch(message->what)
	{
		case MENU_NEW_DEFAULT:
		case MENU_NEW_EMPTY:
			EmptySong();
		break;			
		case MENU_SAVE:		
			jfm->Save(currentSong,false);
		break;
		case MENU_SAVEAS:
			jfm->Save(currentSong,true);
		break;
		case X_REFS_RECEIVED:
			OpenRefs(message);
		break;	
		case MENU_OPEN:
			if(jfm->AskForClose(currentSong) == B_OK) 
				jfm->Load();
		break;
		case MENU_EXPORT:
			main_window->PostMessage(message);
		break;		
		case B_SAVE_REQUESTED:
			if(message->FindRef("directory",&ref)==B_OK)
			{
				jfm->SaveReq(ref,message->FindString("name"),currentSong);
				main_window->Saved();
			}
		break;
		case 'expt':	// EXPORT AS WAVE
		{
			BAlert* idle = XUtils::ShowIdleAlert("Exporting..");
			JFileManager::Get()->RenderAsWave(message, currentSong);
			XUtils::HideIdleAlert(idle);
		}
		break;
		
		case MENU_COPY:
			if(main_window->Lock()) {
				clipboard->setNumberNotes(currentSong->getNumberNotes());
				CopyPattern((track_manager->getCurrentTrack())->getPatternAt(mea_manager->GetCurrentPattern()),clipboard);
				main_window->Unlock();
			}
			main_window->PostMessage(message);
		break;
		case MENU_CUT:
			if(main_window->Lock()) {
				clipboard->setNumberNotes(currentSong->getNumberNotes());
				CopyPattern((track_manager->getCurrentTrack())->getPatternAt(mea_manager->GetCurrentPattern()),clipboard);
				ClearPattern((track_manager->getCurrentTrack())->getPatternAt(mea_manager->GetCurrentPattern()));
				main_window->Unlock();
			}
			main_window->PostMessage(message);
		break;
		case MENU_PASTE:		
			if(main_window->Lock()) {
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
					delete (Pattern*)dinamic_clip->ItemAt(i);	
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
					delete (Pattern*)dinamic_clip->ItemAt(i);	
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
		case SONG_RESET: //used by Panels.cpp after changing Beats and BeatDivision value..
						 // Is there a better alternative?
			if(main_window->Lock())
			{
				main_window->ResetToSong(currentSong);
				main_window->Unlock();
			}

			JuiceEngine::Get()->LockEngine("SongReset");
			JuiceEngine::Get()->ResetSong(currentSong);
			JuiceEngine::Get()->UnlockEngine("SongReset");
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
		to->getNoteAt(i)->SetGain(from->getNoteAt(i)->Gain());	
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
		to->getNoteAt(i)->SetGain(0.8f);	
	}
}
bool
TheApp::QuitRequested()
{
	status_t	ask;
	ask=jfm->AskForClose(currentSong);
	if (ask == B_OK) {
		fVManager->Dump();
	}	
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
				
	if(message->FindRef("refs", &ref) == B_OK)
			LoadSong(ref);
	
}

Song*
TheApp::LoadRequest(entry_ref ref)
{
	Song* new_song = jfm->EmptySong();
	
	//even if it does not exist it's OK. We will apply and empty song!
	jfm->LoadFile(ref, new_song);

	Song* oldSong = currentSong;	
	currentSong = new_song;
	
	return oldSong;
}


void
TheApp::AboutRequested()
{
	(new AboutBox(true))->Show();
}
//-
