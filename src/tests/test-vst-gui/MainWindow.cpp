#include "MainWindow.h"

#include <Application.h>
#include "VSTConfigureView.h"
#include <MenuBar.h>
#include <MenuItem.h>

#define	X_WIN_ZOOM			'xwzo'
#define	X_SAVE_PRESET		'xspt'
#define	X_LOAD_PRESET		'xlpt'
#define	X_SAVE_PRESET_OK 	'xspo'
#define	X_SAVE_LOST			'xslo'

MainWindow::MainWindow(VSTPlugin* plugin)
	:	BWindow(BRect(100,100,500,400),"Main Window",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	VSTConfigureView* config = new VSTConfigureView(plugin);
	SetControls(config, false);
	SetPrograms(plugin->EffectName(),config->GetMenu());
}


void
MainWindow::SetControls(BView* conf, bool scr)
{
	bar = new BMenuBar(BRect(0,0,30,20),"Bar");
	AddChild(bar);
	
	BBox* box=new BBox(BRect(0,bar->Frame().Height()+1,conf->Bounds().Width(),bar->Frame().Height()+conf->Bounds().Height()),"Boxer",B_FOLLOW_NONE);
	ResizeTo(conf->Bounds().Width(),conf->Bounds().Height()+bar->Frame().Height());

	box->AddChild(conf);

	
	AddChild(box);
			
}

void
MainWindow::SetPrograms(const char *name,BMenu* prog)
{
	if (NULL == prog)
		return;
	
	BMenuItem* nameMenu;
	
	bar->AddItem(nameMenu=new BMenuItem(name,new BMessage(X_WIN_ZOOM)));
	nameMenu->SetTarget(this);
	
	BMenu	  *men=new BMenu("Presets");
	BMenuItem *prgs=new BMenuItem(prog);
	//if(prog!=NULL) prgs->AddMenu(prog);
	men->AddItem(prgs);
	/* creazione del menu presez..*/
	
	BMenu* presetz;
	men->AddItem(presetz=new BMenu("User"));
	men->AddSeparatorItem();
	men->AddItem(new BMenuItem("Save as..",new BMessage(X_SAVE_PRESET)));
	//men->AddItem(new BMenuItem("Reload",new BMessage(X_RELOAD_PRESETS)));
	
	//men->SetRadioMode(true);
	
	/* caricamento presets list*/
	//BList	*tlist=new BList(5);
	//VstManager::Get()->FillPresetsMenu(plugin,presetz,X_LOAD_PRESET);
	
	bar->AddItem(men);

}


void
MainWindow::MessageReceived(BMessage *msg)
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


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
