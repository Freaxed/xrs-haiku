/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SamplerTrackBoost.h"

#include <Application.h>
#include <Alert.h>
#include <FilePanel.h>
#include <Message.h>
#include <File.h>
#include <Path.h>
#include <MenuItem.h>
#include <NodeInfo.h>
#include <Alert.h>
#include <FilePanel.h>

#include "SamplerTrack.h"
#include "filemaker_msg.h"
#include "GfxMsg.h"
#include "ExternalManager.h"

#include "Sample.h"
#include "TrackManager.h"
#include "SamplerPanel.h"
#include "XHost.h"

#include "sampler_locale.h"
#include "locale.h"

// DINAMIC CAST!!!!!!!! //

TrackBoost	*booster;

SamplerTrackBoost::SamplerTrackBoost():TrackBoost()
{
	
	name << T_SAMPLER_NAME;
	
	id=0;
			
	menu=new BMenu(T_SAMPLER_NAME_MENU);
		
	panel=new SamplerPanel(this);
	
	extm=new ExternalManager();
	
	TrackManager::Get()->GetXRSDirectoryEntry(&sample_ref,"Samples");
	
	openpanel=NULL;
	
	booster=this;
	
}

SamplerTrackBoost::~SamplerTrackBoost()
{ 
	booster=NULL;
	//TODO:
	delete extm;
}



void SamplerTrackBoost::RemoveAll()
{
	BAlert *sam;
	int32 sam_say;

	if(extm->CountItems()==0) return;
	sam=new BAlert("XRS",T_SAMPLER_REMOVE_ALL_SAMPLES,T_GEN_REMOVE,T_GEN_NO);
	sam_say=sam->Go();
	
	if(sam_say!=0) return; 
	
	
	BList list;
	
	TrackManager::Get()->getAllMyTrack(&list,0);
	

	for(int i=0;i<list.CountItems();i++)
	{
		SamplerTrack* tr=(SamplerTrack*)list.ItemAt(i);
		_secureSetSample(tr,NULL);
		if(tr->isNameLocked()==false) tr->ResetName();
				
	}
	
	extm->MakeEmpty();
	
	MakeMenu();
	
	TrackManager::Get()->refreshAllMyTrack(0);
}
//void
//SamplerTrackBoost::Restart()
//{
////	LogTrace("SamplerTrackBoost RESTART called");	
////	MakeMenu();
//}

void
SamplerTrackBoost::ResetToSong()
{
//	LogTrace("SamplerTrackBoost RESET called");
//	MakeMenu();
}

Track*
SamplerTrackBoost::getTrack()
{
	SamplerTrack *newtrack = new SamplerTrack();
	tracks.AddItem((void*)newtrack);
	return dynamic_cast<Track *>(newtrack);
}

void
SamplerTrackBoost::RemoveMe(SamplerTrack* tr)
{
	tracks.RemoveItem((void*)tr);
	ConsiderToRemove(tr);	
}



status_t
SamplerTrackBoost::_checkPath(const char* p)
{
	BEntry e(p);
	if(e.Exists()) 
		return B_OK;
	
	LogError("[%s] does not exists !\n", p);
	return B_ERROR;
}


void			
SamplerTrackBoost::SaveBoosterSettings(BMessage* data)
{
	BMessage ext;
   		
	for(int j=0;j<getEXTM()->CountItems();j++) {
		ext.AddString("SamplePath", getEXTM()->getSampleAt(j)->GetFullPath());
	}
   		
    data->AddMessage("SamplesList", &ext);
}

void
SamplerTrackBoost::LoadBoosterSettings(BMessage* data)
{
	extm->MakeEmpty();
	
	BMessage	sampleList;
	if (data->FindMessage("SamplesList", &sampleList) != B_OK) 
		return;	
	
	int i = 0;
	BString path;
	while(sampleList.FindString("SamplePath", i++, &path) == B_OK)
	{
		BEntry entry;
		entry.SetTo(path.String());
							
		if ( _checkPath(path.String()) == B_OK )
			_loadSampler(path.String());
	}
	MakeMenu();
	LogTrace("LoadBoosterSettings, loaded %d samples", i-1);
}

status_t
SamplerTrackBoost::_loadSampler(const char* filename)
{
	entry_ref ref;	
	get_ref_for_path(filename, &ref);	
	
	int pos = -1;					
	status_t error = extm->AddSample(ref, &pos);
	LogInfo("Loading Sample [%s] at position [%d]", ref.name, pos);
	return error;
}


ExternalManager*
SamplerTrackBoost::getEXTM()
{
	return extm;
}

status_t
SamplerTrackBoost::RefReceived(entry_ref ref, Track* t, BMessage *m)
{
	SamplerTrack* tr = dynamic_cast<SamplerTrack *>(t);
	
	if(!m || !tr)
		return B_ERROR; 
	
	int pos;	
	status_t status = getEXTM()->AddSample(ref,&pos);
	
	if (status == B_OK)
	{
		Sample *newsample = getEXTM()->getSampleAt(pos);
		ConsiderToRemove(tr);
		_secureSetSample(tr,newsample);
		
		if(!tr->isNameLocked()) 
			tr->ResetName();

		MakeMenu();
		
	}

	return status;
}


void
SamplerTrackBoost::RefreshSelected()
{ 
	TrackManager::Get()->RefreshSelected();
}

void
SamplerTrackBoost::RemoveSample(Sample* s)
{
	if(s==NULL) return;
	
	BAlert *sam;
	int32 sam_say;
	BString text;
	text=T_SAMPLER_REMOVE_SAMPLE_1;
	text << "\n" << s->GetName() << "\n" << T_SAMPLER_REMOVE_SAMPLE_2;
		
	sam=new BAlert("XRS",text.String(),T_GEN_REMOVE,T_GEN_NO);
	sam_say=sam->Go();
	
	if(sam_say!=0) return; 
	
	
	BList list;
	
	TrackManager::Get()->getAllMyTrack(&list,0);
	
	SampleList *sl=extm->getSampleList();
	
	if(sl==NULL) 
		return;
			
	for(int i=0;i<list.CountItems();i++)
	{
		SamplerTrack* tr=(SamplerTrack*)list.ItemAt(i);
		Sample* ss=tr->getSample();
		if(ss == s)
		{
			_secureSetSample(tr,NULL);
			if(tr->isNameLocked()==false) tr->ResetName();
		}
	}
		
	for(int i=0;i<sl->Count();i++) 
	{
		Sample* fs = (*sl)[i];
		if(fs == s)
		{
			sl->Erase(i);
			delete fs;
			break;
		}
	}
	MakeMenu();
	TrackManager::Get()->refreshAllMyTrack(0);
}
void
SamplerTrackBoost::MakeMenu()
{
	menu->RemoveItems(0, menu->CountItems(), true);
	
	LogTrace("Making Sample menu.. %d samples", getEXTM()->CountItems());
		
	for(int16 i=0; i < getEXTM()->CountItems(); i++)
	{
		BMessage *info = new BMessage(TRACK_SAMP_EXT);
		info->AddInt16("sample",i);
		BMenuItem* item = new BMenuItem(getEXTM()->getSampleAt(i)->GetName(), info);
		item->SetTarget(panel);		
		menu->AddItem(item);
		LogTrace("Adding new Sample menu at position %d", i);
	}		
}

void
SamplerTrackBoost::ConsiderToRemove(SamplerTrack* t)
{
	
	if(!t->getSample()) 
		return;
	
	int32 count=tracks.CountItems();
	
	
	for(int i=0;i<count;i++)
	{	
		SamplerTrack* tt=(SamplerTrack*)tracks.ItemAt(i);
		if(t!=tt && t->getSample()==tt->getSample()) 
			return; /* sample is used */
	}
	
	
	/* Removing..*/
	Sample 		*rem=t->getSample();
	SampleList  *sl=extm->getSampleList();
	sl->Remove(rem);
	
	_secureSetSample(t,NULL);
	delete rem;

	MakeMenu();
}
void
SamplerTrackBoost::ChangeSample(int id)
{
	SamplerTrack* tr=dynamic_cast<SamplerTrack *>(getCurrentTrack());
	
	if(tr==NULL || id >= getEXTM()->CountItems()) return;
		
	Sample*	nsamp=getEXTM()->getSampleAt(id);
	
	if(nsamp==tr->getSample()) return;
	
	ConsiderToRemove(tr);
	_secureSetSample(tr,nsamp);
	
	if(tr->isNameLocked()==false) tr->ResetName();
	TrackManager::Get()->RefreshSelected();
}

void
SamplerTrackBoost::_secureSetSample(SamplerTrack* tr,Sample* s)
{
	if(panel->Window()->Lock())
	{
	  XHost::Get()->SendMessage(X_LockSem,NULL);
			tr->setSample(s);
	  XHost::Get()->SendMessage(X_UnLockSem,NULL);
	
	 panel->Window()->Unlock();
	}
}


Sample*
SamplerTrackBoost::FindSample(const char* sam)
{
	Sample *k = NULL;
	//char	base[SAMPLE_NAME_L];
	
	for(int i=0;i<getEXTM()->CountItems();i++)
	{
		if(strcmp((getEXTM()->getSampleAt(i)->GetName()),sam) == 0) 
			 k = getEXTM()->getSampleAt(i);
	}
	
	return k;
}

void
SamplerTrackBoost::LoadSample()
{
	if (!openpanel)
	{
		BMessenger	*me=new BMessenger(panel);
		openpanel = new BFilePanel(B_OPEN_PANEL,me,&sample_ref,0,false,NULL,NULL,true,true);	
	}
	
	openpanel->Show();
	openpanel->GetPanelDirectory(&sample_ref);
}
