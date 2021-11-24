/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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

void
SamplerTrackBoost::Reset()
{
	MakeMenu();
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
	
	extm->Empty();
	
	MakeMenu();
	
	TrackManager::Get()->refreshAllMyTrack(0);
}
void
SamplerTrackBoost::Restart()
{
	//	cancellare i samples dalla memoria.
	//	cancellare il menu
	//	ricreare il menu
	//	altro?
	
	//printf("TrackBoost Restart?\n");	
	extm->Empty();
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
	
	printf("[%s] doesn't exists !\n",p);
	return B_ERROR;
}

void
SamplerTrackBoost::LoadBoosterSettings(BMessage* data){

	int i=0;
	int b=0;
	BString		path;
	entry_ref	ref;
	BMessage	*newdata=new BMessage();
	
		
	if(data->FindMessage("samples_list",newdata)!=B_OK) return;	
	
		
	while(newdata->FindString("Sample_path",i,&path)==B_OK)
	{
		
				
				{// da rendere una funzione!!
				
				
				
				int16 type=-1;
				int32 offset=0;
				
				status_t error;
				
				entry_ref ref;	
				//bool	nodir=true;
				
				error=newdata->FindInt16("Sample_type",i,&type);
				
				if(error!=B_OK) 
					type=1;
				else
					if(type==0)
					{
					 newdata->FindInt32("Sample_offset",b,&offset);
					 b++;
					} 
				// ricerca del sample:
				
				BEntry entry;
				entry.SetTo(path.String());
							
				error=_checkPath(path.String());
				//printf("Checking path #%s# %ld \n",path.String(),path.Length());
				if(error==B_OK)
					_loadSampler(path.String(),type,offset);
				else
				
				{
					BPath  	  p;
					BPath	  p2;
					BEntry	  e;
					entry_ref hope;
					
					data->FindRef("Song_ref",&hope);
					e.SetTo(&hope);
					e.GetParent(&e);
					p.SetTo(&e);
					p2.SetTo(path.String());
					p.Append(p2.Leaf());
								
					error=_checkPath(p.Path());
					if(error==B_OK)
						_loadSampler(p.Path(),type,offset);
					else
					{
						TrackManager::Get()->GetXRSDirectoryEntry(&hope,"Samples");
						e.SetTo(&hope);
						p.SetTo(&e);
						p2.SetTo(path.String());
						p.Append(p2.Leaf());
						error=_checkPath(p.Path());
						if(error==B_OK)
							_loadSampler(p.Path(),type,offset);
						else
						{
							BString error_s;
							error_s.SetTo(T_SAMPLER_CANT_FIND);
							error_s << p.Leaf() <<" \n";
							data->AddString("error",error_s.String());
						
						}
						
					}
					
				}
				i++;
				}
			}

}

status_t
SamplerTrackBoost::_loadSampler(const char* filename,int type,int32 offset)
{
	status_t error=B_ERROR;
	entry_ref ref;
	
	get_ref_for_path(filename,&ref);
	
	printf("LOAD Sample type : %d  Offset %ld \n",type,offset);
	
	if(type==1)
		error=extm->AddSample(ref);
	else 
	if(type==0)
		error=extm->ExtractSample(ref,offset);
						
	return error;	
}
void
SamplerTrackBoost::LoadTrackSettings(Track* trk,BMessage* data){

	SamplerTrack* tr=dynamic_cast<SamplerTrack *>(trk);
	tr->setResampleEnable(data->FindBool("resample_enable"));
	tr->setResample(data->FindInt16("resample"));
	tr->amp=data->FindInt16("ampboost");
	BString name;
	data->FindString("sample_name",&name);
	
	bool b;
	
	
	if(data->FindBool("boost_enable",&b)==B_OK)
		tr->setBoostEnable(b);
	else
		tr->setBoostEnable(false);
	
	if(data->FindBool("reverse",&b)==B_OK)
		tr->setReversed(b);
	else
		tr->setReversed(false);
	
	if(name.Length()<=0) return;
	
	
	Sample *s=FindSample(name.String());
	
	if(s)
	{
		//tr->setSample(s);
		_secureSetSample(tr,s);
		tr->setName(tr->getSample()->name);
	}
	else
	{
			// non so se è bello metterlo qui!
			BString error(T_SAMPLER_CANT_FIND);
			error += name;
			error += '\n';
			data->AddString("error",error.String());
	}
}
	
void
SamplerTrackBoost::SaveTrackSettings(Track* trk,BMessage* data){

	SamplerTrack* tr=dynamic_cast<SamplerTrack *>(trk);
	if(tr->getSample()) data->AddString("sample_name",BString(tr->getSample()->name));
	data->AddBool("resample_enable",tr->isResampleEnable());	
    data->AddInt16("resample",tr->getResample());
	data->AddInt16("ampboost",(int)tr->amp);
	data->AddBool("boost_enable",tr->isBoostEnable());
	data->AddBool("reverse",tr->isReversed());
    	
}

void			
SamplerTrackBoost::SaveBoosterSettings(BMessage* data){

	bool zipped=false;
	
	if(data->FindBool("zipped",&zipped)==B_OK) 
		zipped=true;
	 else
	    zipped=false;
	
	BMessage* ext=new BMessage();
   		
   		for(int j=0;j<getEXTM()->CountItems();j++)
   		{
   			if(zipped && getEXTM()->getSampleAt(j)->type==1)
   			{
   				ext->AddString("Sample_path",getEXTM()->getSampleAt(j)->name);
   				data->AddString("collect",getEXTM()->getSampleAt(j)->path_name);
   			}
   			else
   			 	ext->AddString("Sample_path",getEXTM()->getSampleAt(j)->path_name);
   				
   				
   			ext->AddInt16("Sample_type",getEXTM()->getSampleAt(j)->type);
   			
   			if(getEXTM()->getSampleAt(j)->type==BANK_SAMPLE_TYPE)
   			   			ext->AddInt32("Sample_offset",getEXTM()->getSampleAt(j)->offset);

   		}
   		
    data->AddMessage("samples_list",ext);
}

ExternalManager*
SamplerTrackBoost::getEXTM()
{
	return extm;
}

void
SamplerTrackBoost::_addMD5(const char* filename,BMessage *msg)
{/*
  FILE *inFile = fopen (filename, "rb");
  MD5_CTX mdContext;
  int bytes;
  unsigned char data[1024];

  if (inFile == NULL) {
    printf ("_addMD5: %s can't be opened.\n", filename);
    return;
  }

  MD5Init (&mdContext);
  while ((bytes = fread (data, 1, 1024, inFile)) != 0)
    MD5Update (&mdContext, data, bytes);
  MD5Final (&mdContext);
  
  
  msg->AddString("md5",(const char*)mdContext.digest);
  
  fclose (inFile);*/
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
	text << "\n" << s->name << "\n" << T_SAMPLER_REMOVE_SAMPLE_2;
		
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
	
	BMenuItem	*t;
	BMenuItem * item;
	Sample*        kikko;
	int32 		max;
	BMessage *info;
	

		max=menu->CountItems();
		
		for(int j=0;j<max;j++){
		
		t=menu->ItemAt(0);
		if(t==NULL) break;
		menu->RemoveItem(t);
		delete t;
		}
		
		
				
		for(int zip=0;zip<getEXTM()->CountItems();zip++)
		{
			info=new BMessage(TRACK_SAMP_EXT);
			info->AddInt16("sample",zip);
			kikko=getEXTM()->getSampleAt(zip);
			item=new BMenuItem(kikko->name, info);
			//item->SetTarget(fw);
			item->SetTarget(panel);
			
			menu->AddItem(item);
			
		}
		
		/*menu->AddSeparatorItem();
		info=new BMessage(EXT_SAMPLE);
		info->AddInt16("id",h);
		item=new BMenuItem("Load External Sample...", info);
		item->SetTarget(Window());
		menu->AddItem(item);*/
		
		//popup->AddItem(new BitmapMenuItem("","TrackModePianoOff",NULL));
		
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
		//strcpy(base,(getEXTM()->getSampleAt(i)->name));
		if(strcmp((getEXTM()->getSampleAt(i)->name),sam) == 0) 
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
