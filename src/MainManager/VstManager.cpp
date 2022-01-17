/*
 * 
 * Copyright 2006-2022
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"VstManager.h"
#include 	"VSTItem.h"
#include	"XHost.h"
#include	"Xed_Utils.h"
#include	<Menu.h>
#include	<MenuItem.h>
#include	"Log.h"



#define	VST_PRESET_MIME "audio/XRS-VstPresetFile"


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
	fVstList = new VSTList();
}

void	
VstManager::GetInstrumentVst(BList* outList) {
	for(int32 i=0;i<fVstList->CountPlugins();i++) {
		VSTPlugin* plug = fVstList->GetPluginAt(i);
		if (plug->IsSynth()) {
			LogDebug("Adding VSTInstrument to the list [%s]", plug->EffectName());
			outList->AddItem(plug);
		}
	}
}

void	
VstManager::GetEffectVst(BList* outList){
	for(int32 i=0;i<fVstList->CountPlugins();i++) {
		VSTPlugin* plug = fVstList->GetPluginAt(i);
		if (!plug->IsSynth()) {
			LogDebug("Adding VST Effect to the list [%s]", plug->EffectName());
			outList->AddItem(plug);
		}
	}
}
VstManager::~VstManager()
{
	if(fVstList != NULL) 
	{
		delete fVstList;
		fVstList = NULL;
	}
}

VSTItem*
VstManager::CreateVst(VSTPlugin* fromList)
{
	/*	
	int tempo;
//	XHost::Get()->SendMessage(X_BPM,&tempo);
//	vst2->setBPM(tempo);
	LogError("We have to send tempo to VST");	
	return vst2;*/
	
	return new  VSTItem(fromList);
}
void
VstManager::DeleteVst(VSTItem* plug)
{
	if(plug != NULL){
		delete plug;
	}
}

bool
VstManager::SavePreset(VSTItem* plug,const char*name,BMessage* msg)
{
	return XUtils::SavePreset(BPath(plug->Path()).Leaf(), name, msg, VST_PRESET_MIME);
}
bool					
VstManager::LoadPreset(VSTItem* plug,const char*name,BMessage* msg)
{
	return XUtils::LoadPreset(BPath(plug->Path()).Leaf(), name, msg, VST_PRESET_MIME);
}
void
VstManager::FillPresetsMenu(VSTItem* plug, BMenu* menu, uint32 msg)
{
	XUtils::FillPresetsMenu(BPath(plug->Path()).Leaf(), menu, msg);
}	



