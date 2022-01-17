#include "MixerWindow.h"

#include <Application.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>
#include "MixerLine.h"
#include "WindowManager.h"
#include "CommonValuableID.h"
#include "Log.h"

MixerWindow*	
MixerWindow::Get()
{
 	static	MixerWindow*	instance = NULL;
	if (instance == NULL) 
			instance = new MixerWindow();
	return instance;
}

MixerWindow::MixerWindow(void)
	:	XrsWindow(BRect(150,130,280,60),"", B_FLOATING_WINDOW,B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_AVOID_FOCUS|B_NOT_RESIZABLE)
{
	SetName("mixer_");
	SetTitle("Mixer");
	
	BGroupLayout* group = BLayoutBuilder::Group<>(B_HORIZONTAL);
	SetLayout(group);
	group->SetSpacing(5.0f);
	
	for (uint8 i=0; i<MIXERLINES_COUNT; i++) {
		BString name("Line ");
		name << i;
		if (i == 0) 
			name = "Master";
			
		group->AddView(new MixerLine(name.String(), VID_MIXER_LIN_VOL(i), VID_MIXER_LIN_PAN(i), VID_MIXER_LIN_MET(i)));
	}
	BSize size(group->BasePreferredSize());
	ResizeTo(size.Width(), size.Height());
	
	//windowmanager
	WindowManager::Get()->RegisterMe(this,"Mixer");
}

MixerWindow::~MixerWindow()
{
	WindowManager::Get()->UnregisterMe(this);
}

void
MixerWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

void
MixerWindow::SaveSettings(BMessage*	data)
{
	debugger("TBD");
	/*
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
	LogError("MixerWindow::LoadSettings not implemented");
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

bool
MixerWindow::QuitRequested()
{
	WindowManager::Get()->Switch(this);
	return false;
}
