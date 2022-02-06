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
    SET_INVALID(wave_data[0]);
    SET_INVALID(wave_data[1]);
	channels = UNDEFINED;
}

Sample::~Sample()
{
	if (IS_VALID(wave_data[1])  && GET_BUFFER(wave_data[0]) != GET_BUFFER(wave_data[1])) {
// 		delete[] wave_data[1];
// 		wave_data[1] = NULL;
        DESTROY_BUFFER(wave_data[1]);
	}
	
	if (IS_VALID(wave_data[0])) {
		//delete[] wave_data[0];
		//wave_data[0] = NULL;
        DESTROY_BUFFER(wave_data[0]);
	}

}

void
Sample::PrintToStream()
{
	printf("---------------------------\n");
	printf("Sample Path : %s\n",path_name.String());
	printf("Totatlbyte:	  %ud\n",_totalbytes);
	printf("Channels:	  %d\n",channels);
	printf("Channel[0]: %p\n", GET_BUFFER(wave_data[0]));
	printf("Channel[1]: %p\n", GET_BUFFER(wave_data[1]));
	//printf("Total Frames: %ud\n",frames);
	printf("---------------------------\n");
}
