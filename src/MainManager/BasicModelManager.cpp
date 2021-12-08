/*
 * Copyright 2018, Your Name <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "BasicModelManager.h"
#include "ValuableManager.h"
#include "CommonValuableID.h"


BasicModelManager::BasicModelManager(){
	
	ValuableManager* val_manager = ValuableManager::Get();
	val_manager->RegisterValuable(VAL_ID_BPM, (int32)111);

};

BasicModelManager::~BasicModelManager() {
//	ValuableManager* val_manager = ValuableManager::Get();
//	val_manager->UnRegisterValuable(VAL_ID_BPM);
}
