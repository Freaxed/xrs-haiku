#include <stdio.h>
#include "Valuable.h"
#include "ValuableManager.h"
#include "Log.h"
#include "GenericReceiver.h"
#include "App.h"
#include "ValuableMonitorWindow.h"

int
main() {

	
	Logger::SetLevel(LOG_LEVEL_DEBUG);
	
	GenericReceiver*	gReceiver = new GenericReceiver();
	gReceiver->Run();
	
	
	ValuableManager* vManager = ValuableManager::Get();
		
	
	vManager->RegisterValuable("xrs.mixer.main.volume", 	   	80);
	vManager->RegisterValuable("xrs.time.bpm", 			   (int32)120);
    vManager->RegisterValuable("xrs.mixer.main.vumeter", 0.62f, 0.74f);
    
    

	
	
	vManager->RegisterValuableReceiver("xrs.mixer.main.volume", gReceiver);
	vManager->RegisterValuableReceiver("xrs.time.bpm", 			gReceiver);
	vManager->RegisterValuableReceiver("xrs.mixer.main.vumeter",gReceiver);
	
	
	App *app = new App();
	app->Run();
	
	vManager->UpdateValue("xrs.mixer.main.volume", 97);
	vManager->UpdateValue("xrs.time.bpm", (int32)130);
	vManager->UpdateValue("xrs.mixer.main.vumeter", 0.0f, 9.9f);
	

		
	vManager->UnregisterValuable("xrs.mixer.main.volume");
	vManager->UpdateValue("xrs.mixer.main.volume", 9999);
	
	
	vManager->UnregisterValuableReceiver("xrs.time.bpm", gReceiver);
	vManager->UpdateValue("xrs.time.bpm", (int32)250);
	
	
	vManager->Dump();
	
	
	delete app;

}
