/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SAMPLER_VOICE_
#define _SAMPLER_VOICE_
#include <samplerate.h>
#include "StaticBuffer.h"
#include "pitchtable.h"
#include <assert.h>
#include "TrackSampleRateBuffers.h"

struct SamplerVoice
{
	SamplerVoice(Note* note, Sample* sample, TrackSampleRateBuffers& buffers) : mBuffers(buffers)
	{
		assert(note && sample);		
		
		reverse = false; 
		loop = false;
		this->n 	 = note;
		this->sample = sample;
		fullframes = sample->GetFullframes();

		mFreqFactor = pitchtable[60] / pitchtable[note->getNote()] ;
		
		
		srcState = NULL;
		ResetPosition();
	}
	
	~SamplerVoice() {
		if (srcState != NULL) {
			src_delete (srcState);
			srcState = NULL;
		}
	}
	
	void ResetPosition() 
	{ 
		//start back from zero.
		position = 0; 
		mOutPosition = 0;
		mOutFrames = 0;
		if (srcState != NULL) {
			src_delete (srcState);
			srcState = NULL;
		}
		int error = 0;
		srcState = src_callback_new(SamplerVoice::src_callback, SRC_SINC_FASTEST,  2, &error, this);
	} 
	
	bool GetNextFrames(float* left, float* right)
	{
		if (IsDone()) return false;
		
		uint32 max = mBuffers.mOutBuffer.kFrames / 2;

		if (mOutPosition >= mOutFrames) {
			mOutFrames = (uint32)src_callback_read (srcState, mFreqFactor, max, mBuffers.mOutBuffer.Buffer());
			mOutPosition = 0;
		}
		
		if (mOutPosition < mOutFrames) 
		{
			*left  = mBuffers.mOutBuffer[mOutPosition *2 + 0];
			*right = mBuffers.mOutBuffer[mOutPosition *2 + 1];
			mOutPosition++;
		}

		return !IsDone();
	}
	
	bool	IsDone() {
		return (position >= fullframes);
	}
				

	int					tag;	
	bool				reverse;
	bool				loop;
	
	Note*				GetNote()   { return n;      }
	Sample*				GetSample() { return sample; }
	

	
private:
	
	uint32 				position;
	uint32				fullframes; //useless.. same as sample->fullframes..
	Note*				n;
	Sample*				sample;	
	uint32				mOutPosition;
	uint32				mOutFrames;
	float				mFreqFactor;
	
	TrackSampleRateBuffers&	mBuffers;
	
	SRC_STATE*	srcState;
	
	uint32	get_position()
	{		
		return reverse ? ((fullframes - 1) - position) : position;
	}
	
	
	static long src_callback(void *cb_data, float **dataFloat)
	{
		SamplerVoice*	voice = (SamplerVoice*)cb_data;
		
		uint32  max   = voice->mBuffers.mInBuffer.kFrames / 2;
		
		uint32 	wrote = 0;
		while(wrote < max &&
		      voice->GetNextFramesFromSample(&voice->mBuffers.mInBuffer[wrote*2 + 0], 
		                                     &voice->mBuffers.mInBuffer[wrote*2 + 1])) {
			wrote++;
		}		
		/*
			The callback function should return the number of frames it supplying to the converter. 
			For multi channel files, this return value should be the number of floats divided by the number of channels.
		*/
		*dataFloat = voice->mBuffers.mInBuffer.Buffer();
		return wrote;
	}
	
	bool GetNextFramesFromSample(float* left, float* right)
	{
		if (IsDone()) return false;
		
		
		*left  = sample->wave_data[0][get_position()];
		*right = sample->wave_data[1][get_position()];
		
		position +=1;
		if (loop && IsDone()) {
			position = 0;
		}
		
		return !IsDone();
		
	}
	

		
};

#endif
//--
