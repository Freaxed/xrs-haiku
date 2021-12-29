#include "App.h"
#include "MainWindow.h"
#include "VSTParamWindow.h"
#include "VSTList.h"

App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{

	VSTList *x = new VSTList();
	for (int32 i = 0; i<x->CountPlugins();i++) {
		VSTPlugin*	plug = x->GetPluginAt(i);
		printf("%d %s Input %d Output %d \n", i, plug->EffectName(), plug->Channels(VST_INPUT_CHANNELS), plug->Channels(VST_OUTPUT_CHANNELS));
	}
	printf("DONE.\n");
	
	VSTPlugin*	instance = x->InstantiatePlugin(x->GetPluginAt(1));
	
//	MainWindow *mainwin = new MainWindow(instance);
//	mainwin->Show();
	
	VSTParamWindow* vst = new VSTParamWindow(instance);
	vst->Show();
}


int
main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
