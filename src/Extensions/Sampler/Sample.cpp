/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Sample.h"

Sample::Sample()
{	
	offset = 0;
	wave_data[0] = wave_data[1] = NULL;
	channels = UNDEFINED;
}

Sample::~Sample()
{
	if (wave_data[1] != NULL && wave_data[0] != wave_data[1]) {
		delete[] wave_data[1];
		wave_data[1] = NULL;
	}
	
	if (wave_data[0] != NULL) {
		delete[] wave_data[0];
		wave_data[0] = NULL;
	}

}

void
Sample::PrintToStream()
{
	printf("---------------------------\n");
	printf("Sample Path : %s\n",path_name.String());
	printf("Totatlbyte:	  %ud\n",_totalbytes);
	printf("Channels:	  %d\n",channels);
	printf("Channel[0]: %p\n", wave_data[0]);
	printf("Channel[1]: %p\n", wave_data[1]);
	//printf("Total Frames: %ud\n",frames);
	printf("---------------------------\n");
}
