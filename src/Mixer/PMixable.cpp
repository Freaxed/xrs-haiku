/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PMixable.h"
#include <memory.h>

PMixable::PMixable()
{
	for(int i=0; i<NUM_CHANNELS; i++) 
	{
		stream[i] = new float[MAX_FRAMES];
		fMaxValue[i] = 0.0;
	}
	ClearBuffer();
}

PMixable::~PMixable(){
	
	for(int i=0; i<NUM_CHANNELS;i++)
		delete[] stream[i];
}

void
PMixable::ClearBuffer(int32 size)
{
	for(int i=0; i<NUM_CHANNELS;i++)
		memset((char*)stream[i], 0, size/2);
}

void
PMixable::MixBuffer(float**from, size_t frames, int32 spiaz)
{
	float min[2]={0.0,0.0};
	float max[2]={0.0,0.0};
	for(size_t i =0 ; i<frames ; i++) {
		for(int8 c=0; c<NUM_CHANNELS;c++)
		{
			stream[c][i + spiaz] += from[c][i] * getVolumeChannel(c);
			
			
			if (min[c] > stream[c][i + spiaz])
				min[c] = stream[c][i + spiaz];
			else if (max[c] < stream[c][i + spiaz])
				max[c] = stream[c][i + spiaz];
		}
	}
	for(int8 c=0; c<NUM_CHANNELS;c++){
			if (-max[c] > (min[c] + 1))
			max[c] = -min[c];
		
		fMaxValue[c]=max[c];
	}
}

//void
//PMixable::MixBuffer(float**from, size_t frames, int32 spiaz)
//{
//
//	for(size_t i =0 ; i<frames ; i++)
//		for(int c=0; c<NUM_CHANNELS;c++)
//		{
//			stream[c][i + spiaz] += from[c][i] *vols[c];
//			
//			if ( fabs(stream[c][i + spiaz]) > fMaxValue[i])
//				fMaxValue[c] = fabs(stream[c][i + spiaz]);
//		}
//}

float	
PMixable::GetLastMaxValue(int channel) 
{ 
	//FIX channel>=0
	return channel < NUM_CHANNELS ? fMaxValue[channel] : 0.0 ;
}

/*
void
PMixable::MeltBuffer(float**to,size_t size,float *factors)
{
	//pan-filter??
	for(int c=0; c<NUM_CHANNELS;c++)
		lastop[c]=0;
	
		
	for(size_t i =0 ; i<size ; i++){	
		for(int c=0; c<NUM_CHANNELS;c++) {
		
		stream[c][i] *= factors[c];
		// Do you have a meter??
		if(fabs(stream[c][i])>lastop[c]) lastop[c]=fabs(stream[c][i]);
		
		to[c][i] += stream[c][i];
	 }
	}
}
*/
