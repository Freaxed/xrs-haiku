/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Application.h>
#include <MediaRoster.h>
#include <stdio.h>

BMediaRoster*	gMediaRoster;

int
main()
{
	status_t err;

   BApplication app("application/x-vnd.roster-parser");
   gMediaRoster = BMediaRoster::Roster(&err);

   if (!gMediaRoster || (err != B_OK)) {
      /* the Media Server appears to be dead -- handle that here */
      printf("Bad mediaroster\n");
      return 1;
   }
   media_node outNode;
   if (gMediaRoster->GetAudioOutput(&outNode) != B_OK) {
   		printf("Bad media_node\n");
   		return 0;
   }
	
	printf("media_node_id %d, kind %d, port %d\n", outNode.node, outNode.kind, outNode.port);
	
	if (outNode.kind & B_SYSTEM_MIXER)
		printf("System mixer!\n");
	if (outNode.kind & B_PHYSICAL_OUTPUT)
		printf("B_PHYSICAL_OUTPUT!\n");
	
	media_input	input_array[10];
	int32	outNum = 0;
	if(gMediaRoster->GetConnectedInputsFor(outNode, input_array, 10, &outNum) != B_OK)
	{
		printf("Bad media_inputs\n");
   		return 0;
	}
	
	for (int32 i=0;i<outNum;i++)
	{
		printf("input [%d] - [%s]\n", i, input_array[i].name);
	}

   /* The Media Server connection is in place -- enjoy! */

   return 0;
}

