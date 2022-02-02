/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef StaticBuffer_H
#define StaticBuffer_H


#include <SupportDefs.h>
#include "Log.h"

template<int NbFrames>
class StaticBuffer {
	public:
	
	const uint32	kFrames = NbFrames;
	
	float*		buffer;
	
	status_t	Check()  { return buffer ? B_OK : B_ERROR; }
	float*		Buffer() { return buffer; }
	
	StaticBuffer() {
		buffer = (float*)malloc(NbFrames * sizeof (float));				
		if (buffer == NULL) {
					 LogError ("MediaFileLoader - Error : Out of memory.\n\n") ;
		}
		LogTrace("StaticBuffer ready (%d floats)!", NbFrames);
	}
	~StaticBuffer() {
		if (buffer) {
			LogTrace("StaticBuffer destroyed!");
			free(buffer);
		}
	}
};


#endif // StaticBuffer_H
