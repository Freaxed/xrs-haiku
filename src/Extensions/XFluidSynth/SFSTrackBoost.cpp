#include "SFSTrackBoost.h"

#include <Application.h>
#include <Message.h>
#include <File.h>
#include <Path.h>
#include <Alert.h>

#include "SFSTrack.h"
#include "filemaker_msg.h"
#include "GfxMsg.h"
#include "TrackManager.h"
#include "SFSPanel.h"
#include "SFWindow.h"
#include "XHost.h"

#include "Xed_Utils.h"
#include "sfs_locale.h"

#define	MY_ID	7
#define	MAXCHANNEL	16

SFSPanel*		p;
SFSTrackBoost*	fluid_booster;
SFWindow*		sfwin;

XFSynth			theSynth;
const XFSynth			*ptheSynth=&theSynth;

SFSTrackBoost::SFSTrackBoost():TrackBoost()
{
	fluid_booster=this;
	name <<"XFluidSynth";
	id=MY_ID;

	theSynth.Init();

	menu=new BMenu(T_SFS_NO_SF2);
	p=new SFSPanel();

	sfwin=new SFWindow();
	XHost::Get()->SendMessage(X_Switch,(void*)sfwin);
	XHost::Get()->SendMessage(X_Switch,(void*)sfwin);
}
void		
SFSTrackBoost::Reset()
{
	fluid_booster = this;
		
	BList l;
	TrackManager::Get()->getAllMyTrack(&l,MY_ID);
	int32 count=l.CountItems();
		
	for(int i=0;i<count;i++)
		((SFSTrack*)l.ItemAt(i))->SetHasData(false);
		
	if(count>0) ((SFSTrack*)l.ItemAt(0))->SetHasData(true);
	
	sfwin->Reset();

}

void		
SFSTrackBoost::Restart() {
	
	fluid_booster=NULL;
	LoadSF(NULL);
}

SFSTrackBoost::~SFSTrackBoost()
{
	fluid_booster = NULL;
	theSynth.Reset();
	//Restart();
	if(sfwin && sfwin->Lock()){
		XHost::Get()->SendMessage(X_UnregWindow,(void*)sfwin);
		sfwin->Quit();
	}
}

Track*
SFSTrackBoost::getTrack()
{

	int32 count=tracks.CountItems();
		
	SFSTrack *trk=new SFSTrack();
	
	if(count==0) trk->SetHasData(true);
	else
		trk->setRouteLine(tracks.ItemAt(0)->getRouteLine());

	tracks.AddItem(trk);
	return dynamic_cast<Track *>(trk);
}

void
SFSTrackBoost::ChooseProcessor()
{
	int32 count=tracks.CountItems();
	
	if(count>0)
		tracks.ItemAt(0)->SetHasData(true);
	
}

void
SFSTrackBoost::RemoveMe(Track* tr)
{
	tracks.RemoveItem((SFSTrack*)tr);
	if(tr->HasData()) ChooseProcessor();
	
}


PlugPanel*
SFSTrackBoost::getPanel(){return p;}

void
SFSTrackBoost::SaveTrackSettings(Track* trk,BMessage* data)
{
	
	
	SFSTrack *myTrack=(SFSTrack*)trk;
	data->AddInt16("channel",myTrack->GetChannel());
	
	theSynth.SaveChannelSettings(data,myTrack->GetChannel());
	
	data->AddInt16("reverb_send",myTrack->GetReverbSend());
	data->AddInt16("chorus_send",myTrack->GetChorusSend());
			
}


void			
SFSTrackBoost::LoadTrackSettings(Track* trk,BMessage* data)
{
	SFSTrack *myTrack=(SFSTrack*)trk;
		
	myTrack->SetChannel(data->FindInt16("channel"));
	
	theSynth.LoadChannelSettings(data,myTrack->GetChannel());
	
	myTrack->SetReverbSend(data->FindInt16("reverb_send"));
	myTrack->SetChorusSend(data->FindInt16("chorus_send"));
}
void
SFSTrackBoost::LoadBoosterSettings(BMessage* data)
{
	BString name;
	if(data->FindString("bank_name",&name)==B_OK)
	{
		status_t err=LoadSF(name.String());
		if(err!=B_OK){
		
			BString err;
			err << T_SFS_ERROR_LOADING << " " << name.String() ;
			data->AddString("error",err);
		
		}
	}
	else
		LoadSF(NULL);
		

	BMessage	extra;	
	if(data->FindMessage("extra",&extra)==B_OK);
		sfwin->LoadExtra(&extra);
}

void
SFSTrackBoost::SaveBoosterSettings(BMessage* data)
{
	theSynth.SaveGlobalSettings(data);
	data->AddString("version","0.1"); // ??
	
	
	BMessage	extra;	
	sfwin->SaveExtra(&extra);
	data->AddMessage("extra",&extra);
}
void
SFSTrackBoost::_emptyMenu()
{
	int32 tot;
	tot = menu->CountItems();
	
	BMenu *sub;
	while(menu->CountItems()){
		sub = menu->SubmenuAt(0);
		sub->RemoveItems(0,sub->CountItems()-1,true);
		menu->RemoveItem(sub);
		delete sub;
	}
	
	
	
}

status_t
SFSTrackBoost::LoadSF(const char* filename)
{
	fsynth newsynth;
	newsynth.synth=NULL;
	
	if(!filename) {
	
		theSynth.Reset();
	
	}
	else {
	
	BAlert * al;
	BButton *theb;
	BString text;
	text << T_SFS_LOADING << "\n" << filename;
	al=new BAlert("XRS",text.String(), "...");
	XUtils::SetIdleAlert(al);
	theb=(al->ButtonAt(0));
	theb->Hide();
	al->Go(NULL);
	 
		newsynth = theSynth.LoadFile(filename);
				
		if(al->Lock()) al->Quit();
		
		XHost::Get()->SendMessage(X_LockSem,0);
		
		theSynth.ApplySynth(newsynth);
		
		XHost::Get()->SendMessage(X_UnLockSem,0);
	}
	
	
	_emptyMenu();	
	
	
	theSynth.FillMenu(menu);
	
	for(int i=0;i<menu->CountItems();i++)
		menu->SubmenuAt(i)->SetTargetForItems(p);
		
	
	BPath pathname(filename);
	
	sfwin->Reset(pathname.Leaf());
	
	if(tracks.CountItems() > 0 ){
		p->Refresh();
	}
	return B_OK;
}

void
SFSTrackBoost::SetLine(int line,Track* trk)
{
	//circular implementation
	BList *l=new BList(0);
	TrackManager::Get()->getAllMyJTrack(l,MY_ID);
	int32 count=l->CountItems();
	
	for(int i=0;i<count;i++){
	
		JTrack*	jt=(JTrack*)l->ItemAt(i);
		if(jt->Window() && jt->getTrack()!=trk)
		{
		  BMessage *send=new BMessage(FORCE_ROUTELINE);
		  send->AddInt32("be:value",line);
		  jt->Window()->PostMessage(send,jt);
		  jt->getTrack()->setRouteLine(line);
		}
		
	}
	
}
