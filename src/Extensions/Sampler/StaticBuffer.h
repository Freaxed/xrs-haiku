/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef StaticBuffer_H
#define StaticBuffer_H


#include <SupportDefs.h>
#include "Log.h"
#include <String.h>
#include "FloatBuffer.h"


template<int NbFrames>
class StaticBuffer {

public:
	
	const uint32	kFrames = NbFrames;
	
	FloatBufferPtr		buffer;
	
	status_t	        Check()  { return IS_VALID(buffer) ? B_OK : B_ERROR; }
	float*		        Buffer() { return GET_BUFFER(buffer); }



	
	StaticBuffer()
    {
        CREATE_BUFFER(buffer, NbFrames);
		if (!IS_VALID(buffer)) {
			LogError ("MediaFileLoader - Error : Out of memory.") ;
		}
		LogTrace("StaticBuffer ready (%d floats) (bound_check %d)!", NbFrames, BOUND_CHECK);
	}

	~StaticBuffer() {
		if (IS_VALID(buffer)) {
			LogTrace("StaticBuffer destroyed!");
			DESTROY_BUFFER(buffer);
		}
	}

	inline float   operator [](int i) const    { return buffer[i]; }
    inline float & operator [](int i) 		   { return buffer[i]; }
};


#endif // StaticBuffer_H
