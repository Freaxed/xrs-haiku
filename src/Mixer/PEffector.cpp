/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PEffector.h"
#include "VSTItem.h"
#include <List.h>
#include "VstManager.h"
#include "XHost.h"
#include "Log.h"
#include "LoadingError.h"

PEffector::PEffector():PNode(){
	next = NULL; 
	for(uint8 i=0; i<MAX_EFFECT; i++)
		fVstStack[i] = NULL;

	fFxList = VstManager::Get()->EffectVst();
};

void	
PEffector::SaveSettings(BMessage& fxs) {
	for(uint8 i=0; i<MAX_EFFECT; i++) {
		BMessage effect;		
		effect.AddInt16("Position", (uint8)i);
		if (fVstStack[i] != NULL) {
			effect.AddString("Type", "VST"); //by default
			effect.AddString("Path", fVstStack[i]->Path());
			effect.AddString("EffectName", fVstStack[i]->EffectName());
			BMessage effectData;
			fVstStack[i]->SavePreset(&effectData);
			effect.AddMessage("Settings", &effectData);
		}
		fxs.AddMessage("Effect", &effect);				
	}
}

void
PEffector::LoadSettings(BMessage& msg)
{
	int i = 0;
	BMessage effect;
	while (msg.FindMessage("Effect", i++, &effect) == B_OK)
	{
		uint8 position = effect.GetInt16("Position", MAX_EFFECT);
		if (position >= MAX_EFFECT)
			break;

		VSTPlugin* plugin = NULL;
		BString effectName;
		
		if (effect.FindString("EffectName", &effectName) == B_OK) {
			plugin = FindVST(effectName.String());
			if (!plugin)
			{
				BString what("Can't find the VST plugin [");
				what << effectName.String() << "]!";
				LoadingError::Add("VST Loader", what.String(), "Install the missing VST plugin!");
			}
		}

		VSTItem* newItem = CreateVstAtPosition(plugin, position);
		if (newItem) {
			BMessage effectData;
			effect.FindMessage("Settings", &effectData);
			newItem->LoadPreset(&effectData);
		}
			
	}
}

VSTPlugin*				
PEffector::FindVST(const char* name)
{
	//Search for the effect
	for(int i=0;i<fFxList->CountItems();i++)
	{
		VSTPlugin* plug = (VSTPlugin*)fFxList->ItemAt(i);
		if (strcmp(plug->EffectName(), name) == 0)
		{
			return plug;									
		}
	}

	return NULL;
}

PEffector::~PEffector()
{
	fFxList = NULL;
};

int		
PEffector::PMessage(PNode::pnode_message msg,int val) { 
	
	if(msg == P_TEMPO) {
		for(uint8 i=0; i<MAX_EFFECT; i++)
			if (fVstStack[i])
				fVstStack[i]->setBPM(val);
	}
	
	return PNode::PMessage(msg,val);
}

size_t	
PEffector::Process(float** data,size_t frames) {
	
	SetUsed(false);
	for(uint8 i=0; i<MAX_EFFECT; i++) {
		if (fVstStack[i] &&
			fVstStack[i]->IsActive()) {
			fVstStack[i]->FilterFloat(data, data, (int32)frames, NULL);
			SetUsed(true);
		}
	}

	return frames;
}

VSTItem*	
PEffector::CreateVstAtPosition(VSTPlugin* plugin, uint8 pos)
{
	assert(pos < MAX_EFFECT);

	LogInfo("Loading vst at position %d (%s)", pos, plugin ? plugin->EffectName() : "NULL");
	
	VSTItem* newPlugin = NULL;
	
	if (plugin != NULL)
		newPlugin =	VstManager::Get()->CreateVst( plugin );
	

	VSTItem* toBeRemoved = _LockedSwap(newPlugin, pos);
	
	if (toBeRemoved != NULL)
		VstManager::Get()->DeleteVst( toBeRemoved );

	return newPlugin;
}

VSTItem*		
PEffector::GetVstAtPosition(uint8 pos)
{
	assert(pos < MAX_EFFECT);
	return fVstStack[pos];
}

VSTItem*				
PEffector::_LockedSwap(VSTItem* newPlugin, uint8 pos)
{
	XHost::Get()->LockEngine();
	
	VSTItem* toBeRemoved = fVstStack[pos];
	fVstStack[pos] = newPlugin;
	
	XHost::Get()->UnlockEngine();
	
	return toBeRemoved;
}


//--
