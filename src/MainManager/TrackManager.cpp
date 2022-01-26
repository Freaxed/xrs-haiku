/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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
#include "TrackInfoWindow.h"
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


TrackManager::TrackManager()
{
	curSong=NULL;
	curPanel=NULL;
	for(int i=0;i<MAX_PLUG;i++)
	{
		 image_ar[i]=0;
	}
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
	for(int i=0;i<MAX_PLUG;i++)
	{
		 if(image_ar[i]!=0) unload_add_on(image_ar[i]);
	}
	
}

void
TrackManager::LoadInternalSamplerPlugin(int i)
{
		//TOGLIERE GLI INCLUDE
		TrackBoost * tb = new SamplerTrackBoost();
		list[tb->id]=tb;
		tb->Init();

		BMessage *m = new BMessage(ADD_TRACK);
		m->AddInt16("id",tb->id);
		
		if(tb->id<0 && tb->id>MAX_PLUG-1) 
			return;
	
		myMenu->AddItem(new BMenuItem(tb->name.String(),m,'1'+i));
		
		
		//il TNT
		tb = new TNTrackBoost();
		i = 2;
		
		list[tb->id]=tb;
		tb->Init();

		m = new BMessage(ADD_TRACK);
		m->AddInt16("id",tb->id);
		
		if(tb->id<0 && tb->id>MAX_PLUG-1) 
			return;
	
		myMenu->AddItem(new BMenuItem(tb->name.String(),m,'1'+i));
		
		
		//il VIW
		tb = new VIWTrackBoost();
		i = 4;
		
		list[tb->id]=tb;
		tb->Init();

		m = new BMessage(ADD_TRACK);
		m->AddInt16("id",tb->id);
		
		if(tb->id<0 && tb->id>MAX_PLUG-1) 
			return;
	
		myMenu->AddItem(new BMenuItem(tb->name.String(),m,'1'+i));

		//il XFluidSynth
		tb = new SFSTrackBoost();
		i = 7;
	
		list[tb->id]=tb;
		tb->Init();

		m = new BMessage(ADD_TRACK);
		m->AddInt16("id",tb->id);
		
		if(tb->id<0 && tb->id>MAX_PLUG-1) 
			return;
	
		myMenu->AddItem(new BMenuItem(tb->name.String(),m,'1'+ i ));

		
		
		//il JMDRUM
		tb = new JMDrumTrackBoost();
		i = 8;
		
		list[tb->id]=tb;
		tb->Init();

		m = new BMessage(ADD_TRACK);
		m->AddInt16("id",tb->id);
		
		if(tb->id<0 && tb->id>MAX_PLUG-1) 
			return;
	
		myMenu->AddItem(new BMenuItem(tb->name.String(),m,'1'+ i ));
}

void
TrackManager::LoadPlugin(const char *name,int i)
{
	entry_ref hope;
	GetXRSDirectoryEntry(&hope,"Extensions");
	BPath sup(name); 
	BPath  *p=new BPath(new  BEntry(&hope) );
	p->Append(sup.Leaf());	
	//get_ref_for_path(p->Path(),&hope);
	
	image_ar[i]= load_add_on (p->Path());
	
	if (image_ar[i] > 0)
	{
		// the file is indeed an addon, but is it a VST plugin?
		//printf ("OK! VST Plugin?... ");
		TrackBoost * tb;
		TrackBoost * (*main_plugin) ();
		if (get_image_symbol (image_ar[i], "main_plugin", B_SYMBOL_TYPE_TEXT, (void**) &main_plugin) == B_OK)
		{	// Chances are now that this is a VST plugin, but is it supported?...
									//printf ("Yes!\n");
		tb = (*main_plugin) ();
		printf ("Found XRS-Extension... %10s (id: %2d)\n",tb->name.String(),tb->id);
		
	
		list[tb->id]=tb;
		tb->Init();
		
		
		
		BMessage *m=new BMessage(ADD_TRACK);
		m->AddInt16("id",tb->id);
		
		if(tb->id<0 && tb->id>MAX_PLUG-1) return;
		
		
	
		myMenu->AddItem(new BMenuItem(tb->name.String(),m,'1'+i));
	
		
		
		
		
		
		}
	}
}
Track*
TrackManager::getTrack(int p)
{
	if(list[p]==NULL)
	{
		char x[103];
		sprintf(x,"getTrack() with id  %2d",p);
		ErrorMsg("Strange error appear!! PLEASE report this bug! (oxygenum@tiscali.it)",x);
		return NULL;
	}
	Track *trk=	list[p]->getTrack();
	
	return trk;
}


bool
TrackManager::isBoosterValid(int pos)
{
	if(list[pos]==NULL) return false;
	
	return true;
}


void
TrackManager::SaveBoosterSettings(int i,BMessage* data)
{
	data->AddInt16("id",i);
	list[i]->SaveBoosterSettings(data);
}

void
TrackManager::SaveTrackSettings(Track* trk,BMessage* data)
{
	list[trk->getModel()]->SaveTrackSettings(trk,data);
}
bool
TrackManager::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	bool ret=false;
	
	/*if(t>=MAX_PLUG)
	{
		char x[103];
		sprintf(x,"tm->Load with id  %2d\0",t);
		ErrorMsg("Strange error appears!! PLEASE report this bug! (oxygenum@tiscali.it)",x);
			
		return false;
	}	*/
	//ret=list[t]->Load(cur,t,file,va,rt);
	return ret;
}
	
void
TrackManager::LoadBoosterSettings(BMessage* data)
{
	int i=data->FindInt16("id");
	if(isBoosterValid(i)) list[i]->LoadBoosterSettings(data);
}
void
TrackManager::LoadTrackSettings(Track* trk,BMessage* data)
{
	list[trk->getModel()]->LoadTrackSettings(trk,data);	
}

Track*
TrackManager::SendRef(entry_ref ref, int id,BMessage *m)
{
	if(list[id]==NULL) 	return NULL;
	
	Track* t=getTrack(id);
	
	if(list[id]->RefReceived(ref,t,m)!=B_OK) return NULL;
	
	return t;
}
void
TrackManager::ShowTrackInfo()
{
	WindowManager::Get()->Switch(TrackInfoWindow::Get());
}
JTrack*
TrackManager::MakeJTrack(Track* trk,BRect rect,int16 pos)
{
	return new JTrack(rect,pos);//,trk->getModel());
}

void
TrackManager::Reset(Song* s)
{
	if(curJTrack!=NULL) curJTrack->Deselect();
	
	curJTrack=NULL;
	curSong=s;
	
	for(int i=0;i<MAX_PLUG;i++)
	{
		if(list[i]!=NULL) list[i]->Reset();
	}
	
	
}

void
TrackManager::Restart()
{
	TrackInfoWindow::Get()->Lock();
	if(curJTrack!=NULL) curJTrack->Deselect();
	curJTrack=NULL;
	
	for(int i=0;i<MAX_PLUG;i++)
	{
		if(list[i]!=NULL) list[i]->Restart();
	}	
	TrackInfoWindow::Get()->Unlock();
	curSong=NULL;
	WindowManager::Get()->Hide(TrackInfoWindow::Get());
	
}
void
TrackManager::Close()
{

	
	
	
	for(int i=0;i<MAX_PLUG;i++)
	{
		 if(list[i]!=NULL) delete list[i];
	}

	
}

void
TrackManager::Init()
{
	curJTrack=NULL;
	
	myMenu=new BMenu(T_MENU_EXTENSION);
	for(int i=0;i<MAX_PLUG;i++) list[i]=NULL;
	
	
	LoadInternalSamplerPlugin(0); //"Sampler"
	/*LoadPlugin("Tn306",1);
	LoadPlugin("MidiOut",2);
	LoadPlugin("3Osc",3);
	LoadPlugin("VIW",4);
	LoadPlugin("Generix",5);
	LoadPlugin("XFluidSynth",7);
	LoadPlugin("JMDrum",8);
	*/
	current=NULL;
	
	for(int i=0;i<MAX_PLUG;i++)
	
	if(list[i]!=NULL){
	
			printf("Added panel for %d\n",i);			
			BView *v=(BView*)list[i]->getPanel();
			if(v){
			TrackInfoWindow::Get()->AddChild(v);
			v->Hide();
			}
		}
		
	WindowManager::Get()->Show(TrackInfoWindow::Get());
	WindowManager::Get()->Hide(TrackInfoWindow::Get());
		
	
}

status_t
TrackManager::SelectTrack(JTrack* x) {

	if(curJTrack==x ) return B_ERROR;
	
	if(curJTrack!=NULL) {
		curJTrack->Deselect();
	}
	else	
		 WindowManager::Get()->Show(TrackInfoWindow::Get());
	
	curJTrack=x;
	if(curJTrack!=NULL) 	
	{
		curJTrack->Select();
		if(list[x->getTrack()->getModel()]!=NULL)
		
		if(list[x->getTrack()->getModel()]->getPanel()!=NULL)
		{
			PlugPanel	*v=list[x->getTrack()->getModel()]->getPanel();
			if(TrackInfoWindow::Get()->Lock()){
				if(current) current->Hide();
				current=(BView*)v;
				current->Show();
				TrackInfoWindow::Get()->Unlock();
				TrackInfoWindow::Get()->UpdateIfNeeded();
				TrackInfoWindow::Get()->Activate();
				v->Reset(x->getTrack());
				TrackInfoWindow::Get()->SetTrack(x->getTrack());
				TrackInfoWindow::Get()->UpdateIfNeeded();
			}
			
		}
	}
	else
	{
		WindowManager::Get()->Switch(TrackInfoWindow::Get());
	}	
	
			
	return B_OK;
}
bool
TrackManager::RefReceived(entry_ref ref,JTrack *trk,BMessage *g){

	/* Drag'n Drop! */
	SelectTrack(trk);
	Track*	x=trk->getTrack();
	if(list[x->getModel()]->RefReceived(ref,x,g)==B_OK)
			RefreshSelected();
		
}
void
TrackManager::ResetPanel(Track* x){

	PlugPanel	*v=list[x->getModel()]->getPanel();
	if(v) v->Reset(x);

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
TrackManager::ErrorMsg(const char *msg,const char *txt)
{
	char pippo[strlen(msg)+strlen(txt)+2];
	
	strcpy(pippo,msg);
	strcat(pippo,"\n");
	strcat(pippo,txt); 
	(new BAlert("XRS ", pippo, "Ok!",NULL,NULL,B_WIDTH_AS_USUAL,B_WARNING_ALERT))->Go();	
}

void
TrackManager::getAllMyTrack(BList* l, int id)
{
	if(curSong==NULL) return ;
	
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

