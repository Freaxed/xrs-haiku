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
#include <Path.h>
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
		
		void		CreateBuffer(uint32 fullframes, int channels);
		
		uint32		GetFullframes() {return fullframes; }
		
		void		SetPath(BPath path) { mPath = path; }
		
		const char*		GetFullPath() { return mPath.Path(); }
		const char*		GetName()	  { return mPath.Leaf(); }
		
		
		FloatBufferPtr		wave_data[STEREO];
		
		
			
		uint32		offset;		
		int			modifier;

		int			type;	// 0 is BANK Samples
							// 1 is External Sample //
		
		
		
private:
		uint32		fullframes;
		int			channels;
		BPath		mPath;
};

#endif
