/*
 * Copyright 2018, Your Name <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "BasicModelManager.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"

#define REG(X, Y) ValuableManager::Get()->RegisterValuable(X, Y);


BasicModelManager::BasicModelManager(){
	
	REG(VID_TEMPO_BPM,     (int32)120);
	REG(VID_TEMPO_BEAT,    (int32) -1);
	REG(VID_TEMPO_PATTERN, (int32) -1);
	REG(VID_TEMPO_MEASURE, (int32) -1);
	
	//Mixer
	REG(VID_MIXER_MAIN_VOL, (int32)80);
	

};

BasicModelManager::~BasicModelManager() {
//	ValuableManager* val_manager = ValuableManager::Get();
//	val_manager->UnRegisterValuable(VAL_ID_BPM);
}
