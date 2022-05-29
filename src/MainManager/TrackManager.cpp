/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TrackManager.h"

#include "Xed_Utils.h"
#include "Configurator.h"
#include "filemaker_msg.h"
#include "Note.h"
#include "Xed_Utils.h"
#include "TrackBoost.h"
#include "GfxMsg.h"
#include "XHost.h"
#include "Song.h"
#include "TracksPanel.h"
#include "PlugPanel.h"
#include "WindowManager.h"

#include "SamplerTrackBoost.h" //remove!
#include "TNTrackBoost.h" //remove!
#include "VIWTrackBoost.h" //remove!
#include "JMDrumTrackBoost.h"  //remove! (why?)
#include "SFSTrackBoost.h"  //remove! (why?)

#include <Application.h>
#include <File.h>
#include <Message.h>
#include <Entry.h>
#include <Path.h>
#include <Menu.h>
#include <MenuItem.h>

#include "TrackInfoBox.h"


TrackManager::TrackManager()
{
	curSong=NULL;
	curPanel=NULL;
	for (int i=0;i<MAX_PLUG;i++)
		list[i] = NULL;
}

TrackManager*
TrackManager::Get()
{
	static	TrackManager*	instance = NULL;
	if (instance == NULL) 
			instance = new TrackManager();
	return instance;
}

TrackManager::~TrackManager()
{
	//TODO: delete all the tracks!
}

void
TrackManager::SaveBoosterSettings(BMessage& boostsSettings)
{
	for(int16 i=0;i<MAX_PLUG;i++)
	{
		if(isBoosterValid(i))
		{
			BMessage boost;
			SaveBoosterSettings(i, &boost);
			boostsSettings.AddMessage("BoostSettings", &boost);		
		}
	}
}

void
TrackManager::LoadBoosterSettings(BMessage& msg)
{
	int i=0;
	BMessage boost;
	while(msg.FindMessage("BoostSettings", i, &boost) == B_OK)
	{
		LoadBoosterSettings(&boost);
		i++;
	}
}


status_t		
TrackManager::RegisterTrackBoost(TrackBoost* boost)
{
		int16	id = boost->Id();
		if(id < 0 && id > MAX_PLUG-1) {
			LogError("Invalid TrackBooster id (%d) (%s)", id, boost->Name());
			return B_ERROR;
		}

		list[id] = boost;		
		boost->Init();

		BMessage *m = new BMessage(ADD_TRACK);
		m->AddInt16("id", id);
	
		myMenu->AddItem(new BMenuItem( boost->Name(), m, '1' + (char)id ) );

		LogInfo("TrackBooster [%s] loaded.", boost->Name());
		return B_OK;
}

void
TrackManager::LoadAllTrackBoost()
{
	RegisterTrackBoost ( new SamplerTrackBoost());
	RegisterTrackBoost ( new TNTrackBoost());
	RegisterTrackBoost ( new VIWTrackBoost());
	RegisterTrackBoost ( new SFSTrackBoost());
	RegisterTrackBoost ( new JMDrumTrackBoost());
}


Track*
TrackManager::getTrack(int16 id)
{
	if(!isBoosterValid(id))
	{
		LogError("Not track for ID %d", id);
		return NULL;
	}
	return list[id]->getTrack();
}


bool
TrackManager::isBoosterValid(int id)
{
	return list[id] != NULL;
}


void
TrackManager::SaveBoosterSettings(int16 i, BMessage* data)
{
	data->AddInt16 ("id", i);
	data->AddString("Name", list[i]->Name());
	list[i]->SaveBoosterSettings(data);
}

void
TrackManager::SaveTrackSettings(Track* trk, BMessage* data)
{
	list[trk->getModel()]->SaveTrackSettings(trk,data);
}

	
void
TrackManager::LoadBoosterSettings(BMessage* data)
{
	int16 i = data->GetInt16("id", -1);
	if(isBoosterValid(i)) 
		list[i]->LoadBoosterSettings(data);
}
void
TrackManager::LoadTrackSettings(Track* trk,BMessage* data)
{
	list[trk->getModel()]->LoadTrackSettings(trk,data);	
}

Track*
TrackManager::SendRef(entry_ref ref, int16 id, BMessage *msg)
{
	if(!isBoosterValid(id))
		return NULL;
	
	Track* track = getTrack(id);
	
	if ( list[id]->RefReceived(ref, track, msg) != B_OK ) 
		return NULL;
	
	return track;
}



void
TrackManager::ResetToSong(Song* s)
{
	curSong = s;
	
	SelectTrack(NULL, NULL);	
	
	for(int i=0; i < MAX_PLUG; i++)
	{
		if(isBoosterValid(i)) 
			list[i]->ResetToSong();
	}
}

void
TrackManager::Close()
{
	for(int i=0;i<MAX_PLUG;i++)
	{
		if(isBoosterValid(i)) 
			delete list[i];
	}
}

void
TrackManager::Init()
{
	curJTrack = NULL;
	current   = NULL;

	myMenu = new BMenu(T_MENU_EXTENSION);
	for(int i=0;i<MAX_PLUG;i++) 
		list[i] = NULL;
	
	
	LoadAllTrackBoost();
}

status_t
TrackManager::SelectTrack(JTrack* x, TrackInfoBox* lTrackInfoBox) {

	if ( curJTrack == x && x != NULL) 
		return B_ERROR;
	
	if(curJTrack != NULL) 
		curJTrack->SetSelected(false);

	curJTrack = x;

	if(curJTrack != NULL) 	
	{
		curJTrack->SetSelected(true);
		if(list[x->getTrack()->getModel()] != NULL) {		
			
			PlugPanel*	panel = list[x->getTrack()->getModel()]->getPanel();
			if(panel != NULL)
			{
				if (panel->Parent() == NULL && lTrackInfoBox) {
					lTrackInfoBox->AddChild(panel);
					((BView*)panel)->Hide();
				}
					
				if(lTrackInfoBox && lTrackInfoBox->LockLooper()){
					if(current) 
						current->Hide();
					current=(BView*)panel; 
					current->Show();
					lTrackInfoBox->UnlockLooper();

					panel->ResetToTrack(x->getTrack());
					lTrackInfoBox->SetTrack(x->getTrack());
				}
				LogTrace("After Lock");
				
			}
		}
	}
	else
	{
		if(lTrackInfoBox && lTrackInfoBox->LockLooper()){
			if(current) 
			   current->Hide();
			   
			current = NULL;
			lTrackInfoBox->UnlockLooper();
		} 
			 
	}	
	
			
	return B_OK;
}
bool
TrackManager::RefReceived(entry_ref ref, JTrack *trk, TrackInfoBox* lTrackInfoBox, BMessage *g){

	/* Drag'n Drop! */
	SelectTrack(trk, lTrackInfoBox);
	Track*	x=trk->getTrack();
	if(list[x->getModel()]->RefReceived(ref,x,g)==B_OK)
			RefreshSelected();
	return true;
}
void
TrackManager::ResetPanel(Track* x){

	if (!x) return;
	PlugPanel	*panel =  list[x->getModel()]->getPanel();
	if(panel)  panel->ResetToTrack(x);

}
Track*
TrackManager::getCurrentTrack()
{
	if(curJTrack==NULL) return NULL;
	return curJTrack->getTrack();
}
void
TrackManager::GetXRSDirectoryEntry(entry_ref * ref,const char* folder)
{	
	XUtils::GetXRSDirectoryEntry(ref,folder);
}


void
TrackManager::getAllMyTrack(BList* l, int id)
{
	if(curSong == NULL) 
		return;
	
	for(int i=0;i<curSong->getNumberTrack();i++)
	{
		if(curSong->getTrackAt(i)->getModel()==id)
		l->AddItem((void*)curSong->getTrackAt(i));
	}
}
void
TrackManager::getAllMyJTrack(BList* l, int id)
{
	if(curSong==NULL) return ;
	
	for(int i=0;i<curPanel->getNumberJTrack();i++)
	{
		if(curSong->getTrackAt(i)->getModel()==id)
		l->AddItem((void*)curPanel->getJTrackAt(i));
	}
}
void
TrackManager::refreshAllMyTrack(int id)
{
	if(curPanel==NULL) return ;
	
	for(int i=0;i<curPanel->getNumberJTrack();i++)
	{
		if(curPanel->getJTrackAt(i)->getModel()==id)
		curPanel->Refresh(curPanel->getJTrackAt(i));
	}
	if(curJTrack->getModel()==id) ResetPanel(curJTrack->getTrack());
}

void TrackManager::RefreshSelected()
{
	if(curPanel==NULL) return ;
	curPanel->RefreshSelected();
	ResetPanel(curJTrack->getTrack());
}

