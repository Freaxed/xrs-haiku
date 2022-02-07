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

void
Sample::CreateBuffer(uint32 fullframes, int channels)
{
	for (int m = 0 ; m < channels ; m++) {
		CREATE_BUFFER(wave_data[m], fullframes);
	}
		
	if (channels == 1)
		WRAP_BUFFER(wave_data[0], wave_data[1], fullframes);
		
	this->fullframes = fullframes;
//	this->_totalbytes = fullframes* sizeof(float) * channels;	
	this->type = 1; //EXT_SAMPLE_TYPE;
	this->channels = channels;
}

Sample::~Sample()
{
	if (IS_VALID(wave_data[1])  && GET_BUFFER(wave_data[0]) != GET_BUFFER(wave_data[1])) {
        DESTROY_BUFFER(wave_data[1]);
	}
	
	if (IS_VALID(wave_data[0])) {
        DESTROY_BUFFER(wave_data[0]);
	}

}

void
Sample::PrintToStream()
{
	printf("---------------------------\n");
	printf("Sample Path : %s\n",mPath.Path());
//	printf("Totatlbyte:	  %ud\n",_totalbytes);
	printf("Channels:	  %d\n",channels);
	printf("Channel[0]: %p\n", GET_BUFFER(wave_data[0]));
	printf("Channel[1]: %p\n", GET_BUFFER(wave_data[1]));
	//printf("Total Frames: %ud\n",frames);
	printf("---------------------------\n");
}
