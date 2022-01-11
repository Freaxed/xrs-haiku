#include <stdio.h>

#include "VSTList.h"

int
main(int argc, char** argv){
	bool onlySynth = false;
	if (argc > 1 && argv[1][0] == 's')
			onlySynth = true;
	
	VSTList *x = new VSTList();
	for (int32 i = 0; i<x->CountPlugins();i++) {
		VSTPlugin*	plug = x->GetPluginAt(i);
		bool isSynth = plug->IsSynth();
		if (onlySynth and !isSynth) continue;
		printf("%d %s Input %d Output %d ; IsStnth(%d) \n", i, plug->EffectName(), plug->Channels(VST_INPUT_CHANNELS), plug->Channels(VST_OUTPUT_CHANNELS), (int)isSynth);
	}
	
	for (int32 i = 0; i<x->CountPlugins();i++) {
		VSTPlugin*	plug = x->InstantiatePlugin(x->GetPluginAt(i));
		bool isSynth = plug->IsSynth();
		if (onlySynth and !isSynth) continue;
		printf("%d %s Input %d Output %d ; IsStnth(%d) \n", i, plug->EffectName(), plug->Channels(VST_INPUT_CHANNELS), plug->Channels(VST_OUTPUT_CHANNELS), (int)isSynth);
		for (int j=0;j<plug->ParametersCount();j++) {
			VSTParameter*	para = plug->Parameter(j);
			BString type("Slider");
			if (para->Type() == 2)
				type = "Checkbox";
			else
			if (para->Type() == 3)
				type = "DropList";
				
			printf("   %d -- %s %s type [%s] Min[%s] Max[%s]\n", para->Index(), para->Name(), para->Unit(), type.String(), para->MinimumValue(), para->MaximumValue());
			printf("   Value %f - ListCount %d\n", para->Value(), para->ListCount());
		}
	}
	printf("DONE.\n");
}
