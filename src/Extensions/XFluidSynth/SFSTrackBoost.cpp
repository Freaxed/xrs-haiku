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
#include "LoadingError.h"

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
	name <<"SoundFont";
	id=MY_ID;

	theSynth.Init();

	menu=new BMenu(T_SFS_NO_SF2);
	p=new SFSPanel();

	sfwin=new SFWindow();
	XHost::Get()->SendMessage(X_Switch,(void*)sfwin);
	XHost::Get()->SendMessage(X_Switch,(void*)sfwin);
}
void		
SFSTrackBoost::ResetToSong()
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
SFSTrackBoost::LoadBoosterSettings(BMessage* data)
{
	fluid_booster = NULL;
	
	status_t err = B_OK;
	BString name;

	LoadSF(NULL);
	if(data->FindString("bank_name", &name) == B_OK)
	{
		err = LoadSF(name.String());
	}
	
	if (err != B_OK)
	{
		BString what("Can't find the SoundFont file [");
		what << name.String() << "]!";
		LoadingError::Add("fluidlite", what.String(), "Get the missing SoundFont file!");		
	}

	BMessage	extra;	
	if(data->FindMessage("extra",&extra)==B_OK)
		sfwin->LoadExtra(&extra);
}

void
SFSTrackBoost::SaveBoosterSettings(BMessage* data)
{
	theSynth.SaveGlobalSettings(data);
	
	BMessage	extra;	
	sfwin->SaveExtra(&extra);
	data->AddMessage("extra",&extra);
}
void
SFSTrackBoost::_emptyMenu()
{
	
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
	
		BAlert * al = XUtils::ShowIdleAlert(T_SFS_LOADING);
	 
		newsynth = theSynth.LoadFile(filename);		
		
		XHost::Get()->SendMessage(X_LockSem,0);
		
		theSynth.ApplySynth(newsynth);
		
		XHost::Get()->SendMessage(X_UnLockSem,0);

		if (newsynth.sfont_id == -1)
		{
			BString what("Can't load the SoundFont file [");
			what << filename << "]!";
			LoadingError::Add("SoundFont", what.String(), "Find the missing SoundFont and assign to the right tracks!");
		}

		
		XUtils::HideIdleAlert(al);
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

