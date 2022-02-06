/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <SupportKit.h>
#include "FloatBuffer.h"

enum
{
	UNDEFINED=0,
	MONO = 1,
	STEREO = 2
};

class Sample 
{
	public:
			Sample();
			~Sample();
		
		void		PrintToStream();
		
		FloatBufferPtr		wave_data[STEREO];
		
		BString   	name;
			
		uint32		_totalbytes;
		uint32		fullframes; //number of frames per channel!
		uint32		offset;		
		int			modifier;
		float 		freq_divisor;
		int			type;	// 0 is BANK Samples
							// 1 is External Sample //
		BString		path_name;
		int			channels;

};

#endif
