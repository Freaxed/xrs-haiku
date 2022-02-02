/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef FrequencyConverter_H
#define FrequencyConverter_H


#include <SupportDefs.h>
#include "StaticBuffer.h"

class TrackSampleRateBuffers {

//	Just a couple of buffers to perform the sample rate conversion
//  and to avoid to malloc / free each time a new note is fired!
//  One per Sampler Track.


public:
	StaticBuffer<2048>	mInBuffer; 	//all used!
	StaticBuffer<2048>	mOutBuffer;	//should be all used
};


#endif // _H
