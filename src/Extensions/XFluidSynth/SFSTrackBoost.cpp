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
	fluid_booster = NULL; //?	
	theSynth.LoadSettings(*data);
	UpdateMenu();
}

void
SFSTrackBoost::SaveBoosterSettings(BMessage* data)
{
	theSynth.SaveSettings(*data);
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

void
SFSTrackBoost::LoadSoundFont(const char* filename)
{

	theSynth.LoadSoundFont(filename);
	UpdateMenu();
}

status_t
SFSTrackBoost::UpdateMenu()
{

	_emptyMenu();	
	theSynth.FillMenu(menu);

	LogTrace("SFSTrackBoost:menu:CountItems %d", menu->CountItems());

	for(int i=0;i<menu->CountItems();i++)
	{
		menu->SubmenuAt(i)->SetTargetForItems(p);
	}	
	
	BPath pathname(theSynth.GetSoundFontName());
	
	sfwin->Reset();
	
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

