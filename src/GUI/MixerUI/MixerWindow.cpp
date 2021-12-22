/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include	"MixerWindow.h"
#include	"VSTItem.h"
#include	"BzWindow.h"
#include	"PlugWindow.h"
#include 	"XHost.h"
#include 	"locale.h"
#include	"SourceItem.h"
#include	"AChannelSlider.h"
#include 	"ValuableManager.h"
#include	"Xed_Utils.h"
#include 	"APot.h"
#include	"Xed_Utils.h"
#include	"MixableBox.h"
#include 	"PBus.h"
#include 	"PMixer.h"
#include 	"VstManager.h"
#include	"WindowManager.h"
#include	"Configurator.h"

#include	<StringView.h>
#include 	<stdio.h>
#include	<ScrollView.h>
#include	<ListView.h>
#include	<Button.h>
#include	<PictureButton.h>
#include	"CommonValuableID.h"

#define	VERTICAL_SLIDE	'vers'
#define	VST_STATE		'vsts'
#define	VST_SELECT		'vstt'
#define	SHOW_VST_WINDOW	'svsw'
#define	SET_MIXER_SIZE	'smix'	

//used by the hated BzWindow.

bool 			SetVST(BMessage *msg,void *ck);

/*typedef struct uiblock{
	PBus*			bus;
	PlugWindow*		plugwin[8];		
};*/

// uiblock *ui_list;
// uiblock *selected;

//rgb_color gray,white,black,darkgray;


//#define	HVST	23


MixerWindow*	
MixerWindow::Get()
{
 	static	MixerWindow*	instance = NULL;
	if (instance == NULL) 
			instance = new MixerWindow();
	return instance;
}

MixerWindow::MixerWindow() : XrsWindow(BRect(150,130,280,60),"", B_FLOATING_WINDOW,B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_AVOID_FOCUS|B_NOT_RESIZABLE)
{	
	SetName("mixer_");
	SetTitle("Mixer");

	for (uint8 i=0;i<MIXERLINES_COUNT;i++) {
		BString label = "Line ";
		label << i;
		if (i == 0) 
			label << " (Master)";
			
		AddChild(new MixableBox(BPoint(120*i,0), label, VID_MIXER_LIN_VOL(i), VID_MIXER_LIN_PAN(i), VID_MIXER_LIN_MET(i)));
	}

	ResizeTo(600,240);

	expanded=(bool)Configurator::Get()->FloatKey("mixer_expanded",1);
	
	/* manca extra settaggio !*/
	
	
	//windowmanager
	WindowManager::Get()->RegisterMe(this,"Mixer");
	vst_win = NULL;
}

MixerWindow::~MixerWindow()
{
	if(vst_win && vst_win->Lock())
		vst_win->Quit();
	
	WindowManager::Get()->UnregisterMe(this);
	
	for(int i=0;i<4;i++)
	{
		//Select(i);
		for(int j=0;j<8;j++) 
			DeleteVstWindow(j);
	}
	
	//SaveConfig();
	Config()->PutFloatKey("mixer_expanded",(float)expanded);	
}

bool
MixerWindow::QuitRequested()
{
	WindowManager::Get()->Switch(this);
	return false;
}

void
MixerWindow::Refresh()
{
	int i = 0;
//	for(i=0;i<4; i++) 
//		if(&ui_list[i]==selected) break;
//	selected=NULL;
	//Select(i);
}


void
MixerWindow::MessageReceived(BMessage* msg)
{
	int k;
	
	switch(msg->what){
	
	
	case MSG_VALUABLE_CHANGED:
		//printf("MixerWindow  %ld\n",(int32)bv);
//		ui_list[0].slider->SetAllValue((int32)msg->FindFloat("valuable:value"));
	break;
	case SET_MIXER_SIZE:
	{
		if(expanded)
				ResizeBy(0,small-big);
				
		else
				ResizeBy(0,big-small);	
		expanded=!expanded;
	}
	break;
	case SHOW_VST_WINDOW:
		k=msg->FindInt16("id");
		//if(selected->plugwin[k])
		//	WindowManager::Get()->Switch(selected->plugwin[k]);
	break;
	case VERTICAL_CLICK:
		k=msg->FindInt16("id");
	break;
	case VERTICAL_SLIDE:
		//ASSERT(k>0)
		k=msg->FindInt16("id");
		//ui_list[k].line->setLeftRight((float)ui_list[k].slider->ValueFor(0)/100.,(float)ui_list[k].slider->ValueFor(1)/100.);
	break;
	case 'intm':
		//ASSERT(k>0)
		//k=msg->FindInt16("id");
		//ui_list[k].slider->SetValueFor(0,(int32)(ui_list[k].line->GetLeft()*100.));
		//ui_list[k].slider->SetValueFor(1,(int32)(ui_list[k].line->GetRight()*100.));
	break;
	
	case VST_SELECT:
		k=msg->FindInt16("id");
		VSTSelect(k);
	break;
	case VST_STATE:
		{
			VSTItem *plug;
			k=msg->FindInt16("id");
//			plug=selected->line->vst[k];
				
		}
	break;
	case RESET_MIXER:
		BMessage *data;
		data=new BMessage();
		msg->FindMessage("settings",data);
		LoadSettings(data);
	break;
	default:
	BWindow::MessageReceived(msg);
	break;
	}
}
void
MixerWindow::VSTSelect(int i)
{
/*
	if(vst_win==NULL)
	{
		vst_win=new BzWindow(BRect(200,200,400,600),"nome",B_MODAL_WINDOW,B_NOT_RESIZABLE);
		BBox	 *box=new BBox(BRect(0,0,200,400),"nome");
		box->SetBorder(B_PLAIN_BORDER);
		vst_win->AddChild(box);
		
		box->AddChild(new BStringView(BRect(10,5,190,25),"ciao",T_MIXER_CHOOSE_VST));
	
		list=new BListView(BRect(10,30,190-B_V_SCROLL_BAR_WIDTH,350),"list"); //,"init",new BMessage(112));
	
	
		list->AddItem(new BStringItem(T_MIXER_NONE));
		for(int j=0;j<VstManager::Get()->getList()->CountItems();j++){
			PlugInEntry *ple=(PlugInEntry*)(VstManager::Get()->getList()->ItemAt(j));
			list->AddItem(new SourceItem(ple->name.String(),ple->ref));
		}
	
	box->AddChild(new BScrollView("",list,B_FOLLOW_TOP|B_FOLLOW_LEFT,0,false,true));
	
	
	BMessage *msg=new BMessage(112);
	msg->AddPointer("list",list);
	msg->AddPointer("line",selected->line);
	msg->AddPointer("bus",selected->bus);
	msg->AddInt16("id",i);
	
	ok = new BButton(BRect(10,360,100,380),"name",T_GEN_OK,msg);
	box->AddChild(ok);
	
	msg=new BMessage(112);
	msg->AddPointer("list",list);
	msg->AddPointer("line",selected->line);
	msg->AddPointer("bus",selected->bus);
	msg->AddInt16("id",i);
	list->SetInvocationMessage(msg);
	
	BMessage* msg2=new BMessage(113);
	msg2->AddPointer("list",list);
	
	BButton		*close=new BButton(BRect(101,360,190,380),"name",T_GEN_CLOSE,msg2);
	box->AddChild(close);
	
	this->AddToSubset(vst_win);
	
	vst_win->RedirectMessages(SetVST,(void*)this);
	vst_win->MoveTo(BAlert::AlertPosition(vst_win->Frame().Width(),vst_win->Frame().Height()));
	
	}
	
	BMessage* msg=new BMessage(112);
	msg->AddPointer("list",list);
	msg->AddPointer("line",selected->line);
	msg->AddPointer("bus",selected->bus);
	msg->AddInt16("id",i);
	ok->SetMessage(msg);
	
	msg=new BMessage(112);
	msg->AddPointer("list",list);
	msg->AddPointer("line",selected->line);
	msg->AddPointer("bus",selected->bus);
	msg->AddInt16("id",i);
	list->SetInvocationMessage(msg);
	
	vst_win->Show();*/
}

bool
SetVST(BMessage *msg,void *ck)
{
	MixerWindow*	mix=(MixerWindow*)ck;
	BListView*		list;
	OutputLine*		line;
	PBus*			bus;
	BStringItem*	sel;
	PEffector *efx;
	int k,i;
	
	
			
	switch(msg->what){
	case 113:
		msg->FindPointer("list",(void**)&list);
		list->Window()->Hide();
	break;
	case 112:
		debugger("add vst");
		msg->FindPointer("list",(void**)&list);
		msg->FindPointer("line",(void**)&line);
		msg->FindPointer("bus",(void**)&bus);
		efx=bus->Effector();
		if(!efx) return false;
		
		k=msg->FindInt16("id");
		i=list->CurrentSelection();
		
		if(i >= 0) {
			sel=(BStringItem*)list->ItemAt(i);
		
			if(mix->Lock()){
					
					/*VSTItem	*temp_plug = efx->vst[k];
				
					XHost::Get()->SendMessage(X_LockSem,0);
						line->vst[k]=NULL;
					XHost::Get()->SendMessage(X_UnLockSem,0);
					
					mix->DeleteVstWindow(k);
					VstManager::Get()->DeleteVst(temp_plug);
					*/
					
					//if(i!=0)					
					VSTItem* temp_plug=VstManager::Get()->CreateVst(i-1);
					//else
					//	temp_plug=NULL;
					
					if(temp_plug!=NULL)	
						mix->CreateVstWindow(temp_plug,k);
					
					XHost::Get()->SendMessage(X_LockSem,0);
						efx->AddVST(temp_plug);
					XHost::Get()->SendMessage(X_UnLockSem,0);
					
				
				mix->Refresh();
				mix->Unlock();
				
			}
			list->Window()->Hide();
		}
		
		/*if(i >= 0) {
			sel=(BStringItem*)list->ItemAt(i);
		
			if(mix->Lock()){
					
					VSTItem	*temp_plug=line->vst[k];
				
					XHost::Get()->SendMessage(X_LockSem,0);
						line->vst[k]=NULL;
					XHost::Get()->SendMessage(X_UnLockSem,0);
					
					mix->DeleteVstWindow(k);
					VstManager::Get()->DeleteVst(temp_plug);
					
					
					if(i!=0)					
						temp_plug=VstManager::Get()->CreateVst(i-1);
					else
						temp_plug=NULL;
					
					if(temp_plug!=NULL)	mix->CreateVstWindow(temp_plug,k);
					
					XHost::Get()->SendMessage(X_LockSem,0);
							line->vst[k]=temp_plug;
					XHost::Get()->SendMessage(X_UnLockSem,0);
					
				// opening the window
				mix->Refresh();
				mix->Unlock();
				
			}
			list->Window()->Hide();
		}*/
		
	break;
	default:
	return true;
	}
	return true;
}

void
MixerWindow::CreateVstWindow(VSTItem* vst,int pos)
{/*
	if(vst==NULL || selected==NULL) return;
	
	PlugWindow	*nw=new PlugWindow(vst);
	selected->plugwin[pos]=nw;
	nw->SetTitle(selected->line->getName());
	nw->MoveTo(BAlert::AlertPosition(nw->Frame().Width(),nw->Frame().Height()));
	nw->Show();

	BString reg(selected->line->getName());
	reg << " : " << vst->name.String();
		
	WindowManager::Get()->RegisterMe(nw,reg.String());	
	*/
}
void
MixerWindow::DeleteVstWindow(int pos)
{
	//if(selected==NULL) 
	//	return;
	
	PlugWindow*	nw = NULL; //selected->plugwin[pos];
	
	if(nw == NULL) 
		return;
	
	//selected->plugwin[pos] = NULL;
	
	WindowManager::Get()->UnregisterMe(nw);
	
	if(nw->Lock())
	{
		nw->Quit();
	}
	

	
}
void
MixerWindow::LoadVST(int k,OutputLine* line,int i)
{
	//TODO using new PMixer and VST
	
	/*					
	VSTItem	*temp_plug=line->vst[k];
				
	XHost::Get()->SendMessage(X_LockSem,0);
			line->vst[k]=NULL;
	XHost::Get()->SendMessage(X_UnLockSem,0);
					
	DeleteVstWindow(k);
	VstManager::Get()->DeleteVst(temp_plug);
					
					
		if(i!=0)					
			temp_plug=VstManager::Get()->CreateVst(i-1);
				else
			temp_plug=NULL;
				
			if(temp_plug!=NULL)	CreateVstWindow(temp_plug,k);
					
		XHost::Get()->SendMessage(X_LockSem,0);
				line->vst[k]=temp_plug;
		XHost::Get()->SendMessage(X_UnLockSem,0);
	*/			
		
				
}

void
MixerWindow::Reset()
{
	if(Lock()){
	for(int i=0;i<5;i++)
	{
		
		if(i==0){
			
//			obs_volumes->SetValue(80,T_MIXER_MASTER);
			//ui_list[i].slider->SendValue(0,80);
			//ValuableManager::Get()->UpdateValue("mixer.main", 80.0f);
			
			
		}
		else
			{
			//ui_list[i].line->setVolume(0.8);
			//ui_list[i].line->setPan(0.0);
			_postValue(i);
			
			
			}
	
		//Select(i);
		
		for(int j=0;j<8;j++)
		{
			//LoadVST(j,ui_list[i].line,0);
		}
		
	}
	//Select(0);
	Unlock();
	}
}
void
MixerWindow::SaveSettings(BMessage*	data)
{/*
	data->AddInt16("version",1);
	data->AddInt16("outputlines",5);
	
	BMessage *line=new BMessage();
	BMessage *vst=new BMessage();	
	BMessage *pres=new BMessage();
	BMessage *win=new BMessage();
		
	for(int i=0; i<5; i++)
	{
		line->MakeEmpty();
		vst->MakeEmpty();
		
		line->AddInt16("id",i);
	//	line->AddFloat("vol",ui_list[i].line->getVolume());
	//	if(i!=0) line->AddFloat("pan",ui_list[i].line->getPan());
		
		// ciclo per i preset vst
		for(int j=0;j<MAXPLUG;j++)
		{
			if(ui_list[i].line->vst[j])
			{
				vst->AddInt16("position",j);
				vst->AddString("name",BString(ui_list[i].line->vst[j]->name));
				vst->AddBool("active",ui_list[i].line->vst[j]->GetStatus());
				pres->MakeEmpty();
				ui_list[i].line->vst[j]->SavePreset(pres);
				vst->AddMessage("preset",pres);
				//salvataggio window settings??
				win->MakeEmpty();
				ui_list[i].plugwin[j]->SavePref(win);
				vst->AddMessage("window",win);
			}
		}
		
		data->AddMessage("line_settings",line);
		data->AddMessage("vst_settings",vst);			
	}*/
}
void
MixerWindow::LoadSettings(BMessage*	data)
{
/*
	// clear up vst stack!
	Reset();
	
	int16 ver;
	if(data->FindInt16("version",&ver)!=B_OK)
	{
		return;			
	}
		
	BMessage *line=new BMessage();
	BMessage *vst=new BMessage();	
	BMessage *pres=new BMessage();
	BMessage *win=new BMessage();
	
	BString name;
	int i=0;
	int16 pos=0;
		
	while(data->FindMessage("line_settings",i,line)==B_OK)
	{
			
			
		data->FindMessage("vst_settings",i,vst);
		
		if(line->FindInt16("id",&pos)!=B_OK) pos=i;
		
		if(pos>7) return; //safety
		
		if(Lock())
		{
		Select(pos);
		
		
		if(pos!=0) {
					ui_list[pos].line->setVolume(line->FindFloat("vol"));
					ui_list[pos].line->setPan(line->FindFloat("pan"));
					_postValue(pos);
				
				}
		else
//				obs_volumes->SetValue(80,T_MIXER_MASTER);
		ValuableManager::Get()->SpreadValue("mixer.main",0,80.0);
		
		// ciclo per i preset vst
		int j=0;
		int16 p=0;
		while(vst->FindInt16("position",j,&p)==B_OK)
		{
				pres->MakeEmpty();
				win->MakeEmpty();
				vst->FindMessage("preset",j,pres);
				vst->FindMessage("window",j,win);
				
				int poz=-1;
				
				if(vst->FindString("name",j,&name)==B_OK)
				{
					// New file version (Xrs1.4)
					// the plugin is saved with his iternal name
					// (not the addon filename)
					printf("NAME Vst info (%d) %s pos %d\n",p,name.String(),pos);
					poz = FindVST(name.String());
				}
				else
				if(vst->FindString("leaf",j,&name)==B_OK)
				{
					// Deprecated!
					// Old fileversion (1.2)
					printf("LEAF Vst info (%d) %s pos %d\n",p,name.String(),pos);
					poz = FindVSTOLD(name.String());	
				}
				
				if(poz<0)
				{
					//VST not found!
					BString err;
					err << T_MIXER_CANT_FIND << " " << name.String() << " !\n";
					data->AddString("error",err.String()); 
				}
				else
				{
				LoadVST(p,ui_list[pos].line,poz+1);
				
				if(ui_list[pos].line->vst[p]){
				
					bool b=vst->FindBool("active",j);
					ui_list[pos].line->vst[p]->SetStatus(b);
					//debugger("mixer vst");
					//pres->PrintToStream();
					if(ver==0)
						ui_list[pos].line->vst[p]->LoadPresetOLD(pres); //
					else
						ui_list[pos].line->vst[p]->LoadPreset(pres); //
						
					ui_list[pos].plugwin[p]->LoadPref(win);
										
				}
				}
				j++;
		
		}
		
		
		Unlock();
		}
		line->MakeEmpty();
		vst->MakeEmpty();
		i++;
		
	}
	if(Lock()){
	Select(0);
	Unlock();
	}
*/
}
int
MixerWindow::FindVST(const char* name)
{
	for(int j=0;j<VstManager::Get()->getList()->CountItems();j++)
	{
			PlugInEntry *ple=(PlugInEntry*)(VstManager::Get()->getList()->ItemAt(j));
			if(strcmp(name,ple->name.String())==0) return j;
	}
	return -1;
}

// Deprecated!
int
MixerWindow::FindVSTOLD(const char* name)
{
	for(int j=0;j<VstManager::Get()->getList()->CountItems();j++)
	{
			PlugInEntry *ple=(PlugInEntry*)(VstManager::Get()->getList()->ItemAt(j));
			
			BPath p(ple->ref.name);
			
			printf("FindVST: [%s][%s]\n",name,p.Path());
			
			if(strcmp(name,p.Leaf())==0) return j;
	}
	return -1;
}

void
MixerWindow::_postValue(int line)
{
	BMessage *msg=new BMessage('intm');
	msg->AddInt16("id",line);
	PostMessage(msg);
}
