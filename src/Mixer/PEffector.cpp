/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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


PEffector::PEffector():PNode(){
	next = NULL; 
	for(uint8 i=0; i<MAX_EFFECT; i++)
		fVstStack[i] = NULL;

	fFxList = VstManager::Get()->EffectVst();
};

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
PEffector::CreateVstAtPosition(VSTPlugin* templ, uint8 pos)
{
	assert(pos < MAX_EFFECT);
	
	VSTItem* newPlugin = NULL;
	
	if (templ != NULL)
		newPlugin =	VstManager::Get()->CreateVst( templ );
	

	VSTItem* toBeRemoved = _LockedSwap(newPlugin, pos);
	
	if (toBeRemoved != NULL)
		VstManager::Get()->DeleteVst( toBeRemoved );

	return newPlugin;
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
