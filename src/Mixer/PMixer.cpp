/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PMixer.h"
#include "ValuableManager.h"

PMixer* 
PMixer::Get()
{
	static	PMixer*	instance = NULL;
	if (instance == NULL) 
			instance = new PMixer();
	return instance;
}


PMixer::PMixer(){ 
	for(int i=1; i<MIXERLINES_COUNT; i++){
		BusAt(0)->AddRouted(&busses[i]);	
	}	
	BusAt(0)->ClearBuffer();
}

PMixer::~PMixer()
{

}


void	
PMixer::ResetBuffers()
{
	for(int i=0;i<MIXERLINES_COUNT;i++) {
		if(busses[i].Used())
		{
			busses[i].ClearBuffer();
			busses[i].SetUsed(false);
		}
	}
}
PBus*	
PMixer::GetMain() {
	return BusAt(0);
}
				
PBus*	
PMixer::BusAt(uint8 pos){
	if (pos >= MIXERLINES_COUNT) 
		return NULL;
		
	return &busses[pos];
}
	
//---
