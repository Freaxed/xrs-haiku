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
	next=NULL; 
};

int		
PEffector::PMessage(PNode::pnode_message msg,int val){ 
	
	if(msg == P_TEMPO) {
	//for(int j=0;j<MAX_EFFECT;j++)
	//	if(vst[j]!=NULL)
	//		vst[j]->setBPM(val);
	for(int i=0;i<fVstList.Count();i++)
		fVstList[i]->setBPM(val);
		
		//debugger("FIX");
	}
	
	return PNode::PMessage(msg,val);
}

size_t	
PEffector::Process(float** data,size_t frames){ 
	
	for(int i=0;i<fVstList.Count();i++)
		fVstList[i]->FilterFloat(data, data, frames, NULL);
			
	return frames;
}



//--
