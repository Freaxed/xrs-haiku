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
	SamplerVoice(Note* note, Sample* sample, TrackSampleRateBuffers& buffers)
		:mInBuffer(buffers.mInBuffer) ,mOutBuffer(buffers.mOutBuffer)
	{ 
		assert(note && sample);
		
		
		
		
		reverse = false; 
		loop = false;
		this->n 	 = note;
		this->sample = sample;
		fullframes = sample->fullframes;

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

	int					tag;
	
	
	bool				reverse;
	bool				loop;
	
	Note*				GetNote()   { return n;      }
	Sample*				GetSample() { return sample; }
	
	void				ResetPosition() 
						{ 
							position = 0; 
							mOutPosition = 0;
							if (srcState != NULL) {
								src_delete (srcState);
								srcState = NULL;
							}
							int error = 0;
							srcState = src_callback_new(SamplerVoice::src_callback, SRC_SINC_FASTEST,  2, &error, this);
						} //start back from zero.
	
private:

	StaticBuffer<2048>&	mOutBuffer;	//should be all used
	StaticBuffer<2048>&	mInBuffer; 	//all used!	
	
	uint32 				position;
	uint32				fullframes; //useless.. same as sample->fullframes..
	Note*				n;
	Sample*				sample;	
	uint32				mOutPosition;
	float				mFreqFactor;
	TrackSampleRateBuffers*	mBuffers;
	
	SRC_STATE*	srcState;
	
	uint32	get_position()
	{		
		return reverse ? ((fullframes - 1) - position) : position;
	}
	
	
	static long src_callback(void *cb_data, float **dataFloat)
	{
		SamplerVoice*	voice = (SamplerVoice*)cb_data;
		
		uint32  max   = voice->mInBuffer.kFrames / 2; //fullframes
		float*	data  = voice->mInBuffer.Buffer();
		
		uint32 	wrote = 0;
		while(voice->GetNextFramesOld(&data[wrote*2 + 0], &data[wrote*2 + 1]) && wrote < max) {
			wrote++;
		}		
		/*
			The callback function should return the number of frames it supplying to the converter. 
			For multi channel files, this return value should be the number of floats divided by the number of channels.
		*/
		*dataFloat = data;
		return wrote;
	}
			
public:
	bool GetNextFrames(float* left, float* right)
	{
		if (is_done()) return false;
		
		uint32 max = mOutBuffer.kFrames / 2; //fullframes?;

		if (mOutPosition == 0) {
			mOutPosition = (uint32)src_callback_read (srcState, mFreqFactor, max, mOutBuffer.Buffer());
		}
		
		if (mOutPosition != 0) {
			*left  = mOutBuffer.Buffer()[mOutPosition*2 + 0];
			*right = mOutBuffer.Buffer()[mOutPosition*2 + 1];
			mOutPosition--; //are we thinking in frames or fullframes?
		}

		return !is_done();
	}

	bool GetNextFramesOld(float* left, float* right)
	{
		if (is_done()) return false;
		
		
		*left  = sample->wave_data[0][get_position()];
		*right = sample->wave_data[1][get_position()];
		
		position +=1;
		if (loop && is_done()) {
			ResetPosition();
		}
		
		return !is_done();
		
	}
	
//	bool GetNextFrames(float* left, float* right)
//	{
//		if (is_done()) return false;
//		
//		*left  = sample->wave_data[0][get_position()];
//		*right = sample->wave_data[1][get_position()];
//		
//		position +=1;
//		if (loop && is_done()) {
//			ResetPosition();
//		}
//		
//		return !is_done();
//		
//	}

	bool	is_done() {
		return (position >= fullframes && mOutPosition == 0);
	}
};

#endif
//--
