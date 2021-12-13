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


PMixer::PMixer():PBus(){ 
	
	//defaut routing table..
	
	ValuableManager::Get()->RegisterValuable("xrs.mixer.main.volume",  80);
	ValuableManager::Get()->RegisterValuable("xrs.mixer.main.pan",  0);
//	ValuableManager::Get()->RegisterValuable("xrs.mixer.main.meter",  80);
	
	SetName("Master");
	
	for(int i=0; i<NUM_BUSSES; i++){
		
		AddRouted(&busses[i]);
		busses[i].SetUsed(false);
		
		BString valuableName("xrs.mixer.line.");
		valuableName << i << ".volume";
		
		ValuableManager::Get()->RegisterValuable(valuableName.String(), 80);
		
		BString panID("xrs.mixer.line.");
		panID << i << ".pan";
		
		ValuableManager::Get()->RegisterValuable(panID.String(), 0);
		
	
	}
	
	ClearBuffer();
	
}

PMixer::~PMixer()
{
	//TODO unregistering all and delete.

}


void	
PMixer::ResetBuffers()
{
	if (Used())
	{
		ClearBuffer();
		SetUsed(false);
	}
	
	for(int i=0;i<NUM_BUSSES;i++)
		if(busses[i].Used())
		{	
			busses[i].ClearBuffer();
			busses[i].SetUsed(false);
		}
}
				
PBus*	
PMixer::BusAt(uint pos){
	if(pos>=NUM_BUSSES) return NULL;
	return &busses[pos];
}
	
//---
