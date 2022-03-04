/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef FloatBuffer_H_
#define FloatBuffer_H_

#include "version.h"



#if BOUND_CHECK

class FloatBuffer {

public:
    ~FloatBuffer()
    {
         if (buffer && !wrapped) {
         	printf("Warning! buffer object destroyed but not freed!\n");
         	Delete();
         }
    }

    float*  buffer;
    int   	NbFrames;
    bool	wrapped;

    float test;

    FloatBuffer(): buffer(NULL), NbFrames(0), wrapped(false)
    {
    }

    void Create(uint32   frames)
    {
        buffer = new float[frames];
        NbFrames = frames;
        printf("Created buffer.. %p %d\n", buffer, frames);
    }

    void Delete()
    {
        if (buffer && !wrapped) {
        	printf("Freeing buffer.. %d\n", NbFrames);
        	delete[] buffer; 
        	buffer = NULL; 
        	wrapped = false;
        }
        if (wrapped)
        	printf("NOT (wrapped) Freeing buffer.. %d\n", NbFrames);
    }
    
    void Wrap(void* data, int frames)
    {
    	buffer = (float*)data; 
    	NbFrames = frames;
    	wrapped = true;
    }
    
    void Wrap(FloatBuffer& data, int frames)
    {
    	buffer = data.buffer; 
    	NbFrames = frames;
    	wrapped = true;
    }


	float   operator [](int i) const
	{
		if (i < 0 || i >= NbFrames) {
			printf("Security error: buffer out of bounds! (%d not in 0-%d)\n", i, NbFrames);
			debugger("buffer out of bounds!");
		}

		return buffer[i];
	}

	float & operator [](int i)
	{
		if (i < 0 || i >= NbFrames) {
			printf("Security error: buffer out of bounds! (%d not in 0-%d)\n", i, NbFrames);
			debugger("buffer out of bounds!");
		}

		return buffer[i];
	}
};

#define FloatBufferPtr  FloatBuffer
#define GET_BUFFER(FBP) FBP.buffer
#define CREATE_BUFFER(FBP, FRAMES) FBP.Create(FRAMES);
#define DESTROY_BUFFER(FBP) FBP.Delete(); GET_BUFFER(FBP) = NULL;
#define WRAP_BUFFER(DATA, FBP, FRAMES) FBP.Wrap(DATA, FRAMES)
#else

/*
 * Production version of a FloatBufferPtr = float*
 */

#define FloatBufferPtr float*
#define GET_BUFFER(FBP) FBP
#define CREATE_BUFFER(FBP, FRAMES) FBP = new float[FRAMES]
#define DESTROY_BUFFER(FBP) delete[] FBP; GET_BUFFER(FBP) = NULL;
#define WRAP_BUFFER(DATA, FBP, FRAMES) GET_BUFFER(FBP) = (float*)DATA
#endif

// common functions

#define IS_VALID(FBP) (GET_BUFFER(FBP) != NULL)
#define SET_INVALID(FBP) GET_BUFFER(FBP) = NULL

#endif //FloatBuffer_H_
