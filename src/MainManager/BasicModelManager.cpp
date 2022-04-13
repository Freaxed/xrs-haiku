/*
 * Copyright 2006-2022, Andrea Anzani.
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "BasicModelManager.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"

#define REG(X...)    ValuableManager::Get()->RegisterValuable(X);
#define SET(X...)    ValuableManager::Get()->UpdateValue(X);

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

	REG(VID_PATTERN_CURRENT, (int32)0);
};

BasicModelManager::~BasicModelManager() {}

void	
BasicModelManager::ResetValues() 
{
	SET(VID_TEMPO_BPM,     (int32)120);
	SET(VID_TEMPO_BEAT,    (int32) -1);
	SET(VID_TEMPO_PATTERN, (int32) -1);
	SET(VID_TEMPO_MEASURE, (int32) -1);
	
	//Mixer
	for (uint8 i=0; i<5; i++) {
		SET(VID_MIXER_LIN_VOL(i), (int32)80);
		SET(VID_MIXER_LIN_PAN(i), (int32)00);
		SET(VID_MIXER_LIN_MET(i), (float)0.0f, (float)0.0f);
	}

	SET(VID_PATTERN_CURRENT, (int32)0);
}
