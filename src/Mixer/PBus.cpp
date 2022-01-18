/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PBus.h"

#include <stdio.h>

size_t	
PBus::Process(float** ,size_t frames){
	return 0;
}


void	
PBus::Process(PBus* caller,size_t frames)
{
	for (int i=0;i< fRouted.Count(); i++)
	{
		fRouted[i]->Process(this,frames);	
		if(fRouted[i]->Used())
			SetUsed(true);
	}
	
	fEffector.Process(stream,frames);
	if(fEffector.Used()) SetUsed(true);			
	
	if (caller && Used())
	{
	  caller->MixBuffer(stream, frames, 0);
	}
}

int		
PBus::PMessage(PNode::pnode_message msg,int val){
	fEffector.PMessage(msg,val);	
	return PMixable::PMessage(msg,val);
}

//--
