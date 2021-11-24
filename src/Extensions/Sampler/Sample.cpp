/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Sample.h"

Sample::Sample()
{	
	path_name = NULL;
	offset = 0;
	wave_data = NULL;
	channels = UNDEFINED;
	freq_divisor = 1.0;
}

Sample::~Sample()
{
	if (wave_data != NULL) 
		free(wave_data);
	
	if (path_name != NULL) 
		free(path_name);
}

void
Sample::PrintToStream()
{
	printf("---------------------------\n");
	printf("Sample Path : %s\n",path_name);
	printf("Totatlbyte:	  %ld\n",_totalbytes);
	printf("Channels:	  %d\n",channels);
	printf("Total Frames: %ld\n",frames);
	printf("freq_divisor: %f\n",freq_divisor);
	printf("---------------------------\n");
}
