#include "App.h"
#include "MainWindow.h"
#include "VSTParamWindow.h"
#include "VSTList.h"

App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	int minPars = 99999;
	int maxPars = -1;
	int maxId = -1;
	int minId = -1;
	VSTList *x = new VSTList();
	for (int32 i = 0; i<x->CountPlugins();i++) {
		VSTPlugin*	plug = x->GetPluginAt(i);
		printf("%d %s Input %d Output %d \n", i, plug->EffectName(), plug->Channels(VST_INPUT_CHANNELS), plug->Channels(VST_OUTPUT_CHANNELS));
		int numPars = plug->ParametersCount();
		if (numPars > maxPars) { maxPars = numPars; maxId = i; }
		if (numPars < minPars) { minPars = numPars; minId = i; }
	}
	printf("DONE.\n");
	
	(new VSTParamWindow(x->InstantiatePlugin(x->GetPluginAt(maxId))))->Show();
	
	VSTPlugin* clone = x->InstantiatePlugin(x->GetPluginAt(minId));
	(new VSTParamWindow(clone))->Show();
	(new VSTParamWindow(clone))->Show();
	
}


int
main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
