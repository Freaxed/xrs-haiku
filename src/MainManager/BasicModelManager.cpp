/*
 * Copyright 2018, Your Name <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "BasicModelManager.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"

#define REG(X...)    ValuableManager::Get()->RegisterValuable(X);


BasicModelManager::BasicModelManager(){
	
	REG(VID_TEMPO_BPM,     (int32)120);
	REG(VID_TEMPO_BEAT,    (int32) -1);
	REG(VID_TEMPO_PATTERN, (int32) -1);
	REG(VID_TEMPO_MEASURE, (int32) -1);
	
	//Mixer
	for (uint8 i=0; i<5; i++) {
		REG(VID_MIXER_LIN_VOL(i), (int32)80);
		REG(VID_MIXER_LIN_PAN(i), (int32)00);
		REG(VID_MIXER_LIN_MET(i), (float)0.0f, (float)0.0f);
	}
};

BasicModelManager::~BasicModelManager() {
//	ValuableManager* val_manager = ValuableManager::Get();
//	val_manager->UnRegisterValuable(VAL_ID_BPM);
}
