#include <stdio.h>

#include "VSTList.h"

int
main(){
	VSTList *x = new VSTList();
	for (int32 i = 0; i<x->CountPlugins();i++) {
		VSTPlugin*	plug = x->GetPluginAt(i);
		printf("%s Input %d Output %d \n", plug->EffectName(), plug->Channels(VST_INPUT_CHANNELS), plug->Channels(VST_OUTPUT_CHANNELS));
	}
	printf("DONE.\n");
}
