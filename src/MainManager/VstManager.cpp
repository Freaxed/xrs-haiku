/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"VstManager.h"
#include 	"VSTItem.h"
#include	"Configurator.h"
#include	"XHost.h"
#include 	"FindDirectory.h"
#include	"Xed_Utils.h"
#include 	<List.h>
#include	<Entry.h>
#include	<Directory.h>
#include	<File.h>
#include	<NodeInfo.h>
#include	<Menu.h>
#include	<MenuItem.h>
#include	"Log.h"
#include 	<stdio.h>


#define	VST_PRESET_MIME "audio/XRS-VstPresetFile"
/*move!*/
BDirectory	xdir;
BPath xpath;

extern BList* vst_list;

int Sort(const void * first,const void *second)
{
	return (*(const PlugInEntry**)first)->name.Compare((*(const PlugInEntry**)second)->name);
}


VstManager*	
VstManager::Get()
{
	 static	VstManager*	instance = NULL;
	if (instance == NULL) 
			instance = new VstManager();
	return instance;
}

VstManager::VstManager() 
{
	BEntry e;
	entry_ref rif;
			
	if(vst_list == NULL) 
		vst_list=new BList(10); //leave it as is!

	//XRS folder:

	BPath vstxrs;
	XUtils::GetXRSDirectoryEntry(&rif,"VST");
		
	e.SetTo(&rif);
	e.GetPath(&vstxrs);
		
	VSTItem::load_plug_ins (vstxrs.Path());
	
	// (/boot/home/config/add-ons/media/vstplugins)
     //	/boot/system/add-ons/media/vstplugins
	
	BPath vstaddon("/boot/system/add-ons/media/vstplugins");
//	find_directory (B_USER_ADDONS_DIRECTORY, &vstaddon, true);
//	vstaddon.Append("media");
//	vstaddon.Append("vstplugins");
		
	VSTItem::load_plug_ins (vstaddon.Path());
		
	LogInfo("Loading pluing in path [%s]", vstaddon.Path());
		
	//TODO: we should find a way to avoid already loaded VST.
		
	/* Sort items! */
		
	vst_list->SortItems(Sort);
		
	for(int i=0;i<vst_list->CountItems();i++)
		 if( ((PlugInEntry*)(vst_list->ItemAt(i)))->isSynth==false)
				list.AddItem(vst_list->ItemAt(i));
			
	
	/* presets directory */
		
	find_directory (B_USER_SETTINGS_DIRECTORY, &xpath, true);
	xpath.Append ("XRSPresets");
	
	status_t err = xdir.SetTo(xpath.Path());

	if(err == B_ENTRY_NOT_FOUND)
	{
		create_directory(xpath.Path(),0777);
		xdir.SetTo(xpath.Path());
	}
	
}


VstManager::~VstManager()
{
	if(vst_list!=NULL) 
	{
		delete vst_list;
		vst_list=NULL;
	}
}

VSTItem*
VstManager::CreateVst(int pos)
{
		
	if(pos > list.CountItems()-1) 
		return NULL;
			
	PlugInEntry *ple=(PlugInEntry*)(list.ItemAt(pos));
	VSTItem* vst2=new  VSTItem(ple->ref.name);
	
	int tempo;
//	XHost::Get()->SendMessage(X_BPM,&tempo);
//	vst2->setBPM(tempo);
	debugger("We have to send tempo to VST");	
	return vst2;
}
void
VstManager::DeleteVst(VSTItem* plug)
{
	/* salvataggio su disco dei presetz!*/
	
	if(plug!=NULL){
		//SavePresets printf("Sto chiudendo %s\n",plug->filename.String());
		//BString sp;
		//vst->user_presets->FindString("presets_path",&sp);
		delete plug;
	}
}

bool
VstManager::SavePreset(VSTItem* plug,const char*name,BMessage* msg)
{
	//BEntry	e;
	status_t err;
	BPath	path(plug->presets_path.String());
	err=xdir.SetTo(path.Path());
	if(err==B_ENTRY_NOT_FOUND){
		create_directory(path.Path(),0777);
		xdir.SetTo(path.Path());
	}

	BFile	*file=new BFile();
	path.Append(name);
	printf("SAVE full filename %s\n",path.Path());
	
	/* qua manca il controllo se il file esite già.. */

	BEntry controllo_esiste(path.Path());
	if(controllo_esiste.Exists()==true) {
	
		printf ("file error %ld\n",err);
		delete file;
		return false;
	}
	
	err=file->SetTo(path.Path(),B_WRITE_ONLY|B_CREATE_FILE);
	
	if(err!=B_OK) {
	
		printf ("file error %ld\n",err);
		delete file;
		return false;
	}
	
	msg->Flatten(file);
	
	AddMime(file);
	delete file;
	return true;
}
bool					
VstManager::LoadPreset(VSTItem* plug,const char*name,BMessage* msg)
{
	status_t err;
	BPath	path(plug->presets_path.String());
	
	BFile	*file=new BFile();
	path.Append(name);
	printf("LOAD full filename %s\n",path.Path());
	err=file->SetTo(path.Path(),B_READ_ONLY);
	
	if(err!=B_OK) {
		delete file;
		return false;
	}
	
	if(!CheckMimeType(file)) {
		printf ("wrong file type %ld\n",err);
		delete file;
		return false;
	}
		
	msg->Unflatten(file);
	//msg->PrintToStream();
	delete file;
	
	return true;
}
void
VstManager::FillPresetsMenu(VSTItem* plug, BMenu* menu, uint32 msg)
{
	xdir.SetTo(plug->presets_path.String());
	BEntry e;
	entry_ref rif;

	while(xdir.GetNextEntry(&e,true)==B_OK)
	{
		e.InitCheck();		
		if(e.IsFile())
		{
			e.GetRef(&rif);
			menu->AddItem(new BMenuItem(rif.name,new BMessage(msg)));
		}		
	}
}	
void
VstManager::setPresetsPath(VSTItem* plug)
{
	BPath	path(plug->filename_path.String());
	BPath	zpath(xpath);
	
	BEntry	e;
	zpath.Append(path.Leaf());
	printf("presez name %s \n",zpath.Path());
	plug->presets_path = zpath.Path();
}

void
VstManager::AddMime(BFile* file)
{
	BNodeInfo	info;
	info.SetTo(file);
	info.SetType(VST_PRESET_MIME);
}

bool
VstManager::CheckMimeType(BFile* file)
{
	BNodeInfo	info;
	status_t 	err;
	err=info.SetTo(file);
	char mime[100]; //FIX: max mime string size!
	err=info.GetType(mime);

	return (strcmp(mime,VST_PRESET_MIME)==0);
}
