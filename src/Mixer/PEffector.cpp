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

PEffector::~PEffector(){ 
};

PEffector::PEffector():PNode(){
	next = NULL; 
	for(uint8 i=0; i<MAX_EFFECT; i++)
		fVstStack[i] = NULL;
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



//--
