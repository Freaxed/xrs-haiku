/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SAMPLER_VOICE_
#define _SAMPLER_VOICE_

class SampleRefiller;
class Resampler;

struct SamplerVoice
{
	int					tag;
	uint32 				position;
	Note*				n;
	Sample*				sample;
	
	bool					reverse;
	
	//
	Resampler* 			converter;
	SampleRefiller*		refiller;
};

#endif
//--
