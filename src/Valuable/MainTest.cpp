#include <stdio.h>
#include "Valuable.h"
#include "ValuableManager.h"
#include "Log.h"
#include "GenericReceiver.h"


int
main() {
	Logger::SetLevel(LOG_LEVEL_DEBUG);
	
	GenericReceiver*	gReceiver = new GenericReceiver();
	gReceiver->Run();
	
	
	ValuableManager* vManager = ValuableManager::Get();
	vManager->Run();
	
	vManager->RegisterValuable("xrs.mixer.main.volume", 	   	0.8f);
	vManager->RegisterValuable("xrs.time.bpm", 			   (int32)120);
    vManager->RegisterValuable("xrs.mixer.main.vumeter", 0.62f, 0.74f);
	
	vManager->RegisterValuableReceiver("xrs.mixer.main.volume", gReceiver);
	vManager->RegisterValuableReceiver("xrs.time.bpm", 			gReceiver);
	vManager->RegisterValuableReceiver("xrs.mixer.main.vumeter",gReceiver);
	
	getc(stdin);
	
	vManager->UpdateValue("xrs.mixer.main.volume", 9.7f);
	
	getc(stdin);
	
	vManager->UpdateValue("xrs.time.bpm", (int32)130);
	
	getc(stdin);
	
	vManager->UpdateValue("xrs.mixer.main.vumeter", 0.0f, 9.9f);
	
	getc(stdin);
		
	vManager->UnregisterValuable("xrs.mixer.main.volume");
	vManager->UpdateValue("xrs.mixer.main.volume", 9999.7f);
	
	getc(stdin);
	
	vManager->UnregisterValuableReceiver("xrs.time.bpm", gReceiver);
	vManager->UpdateValue("xrs.time.bpm", (int32)250);
	
	getc(stdin);
	
	vManager->Dump();
	
	getc(stdin);

}
