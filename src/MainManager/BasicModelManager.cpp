/*
 * Copyright 2018, Your Name <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "BasicModelManager.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"


BasicModelManager::BasicModelManager(){
	
	ValuableManager* val_manager = ValuableManager::Get();
	fVBpm = new GenericValuable<int16>();
	fVBpm->SetValue(0, 119);
	val_manager->RegisterValuable(VAL_ID_BPM, fVBpm);

};

BasicModelManager::~BasicModelManager() {
	ValuableManager* val_manager = ValuableManager::Get();
	val_manager->UnRegisterValuable(VAL_ID_BPM);
	delete fVBpm;
	fVBpm = NULL;	
}
