/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef StereoVolumes_H
#define StereoVolumes_H


#include <SupportDefs.h>
#include "Log.h"


class StereoVolumes {
public:
		StereoVolumes(){ fPan = 0 ; fGain = 100; updateVolumes(); }

	void SetGain(float gain){ fGain = gain; updateVolumes(); }
	void SetPan(float pan)  { fPan  = pan ; updateVolumes(); }


	float getVolumeChannel(int8 ch) { return fVolumes[ch]; }

private:

	void updateVolumes(){				 
			fVolumes[0] = fGain;
			fVolumes[1] = fGain;

		if(fPan > 0) 
			fVolumes[0] *= (1.0f - fPan);
		else
		if(fPan < 0) 
			fVolumes[1] *= (1.0f + fPan);
		
		LogTrace("[StereoVolumes] fVolumes[%f,%f] fGain[%f] fPan[%f]", fVolumes[0], fVolumes[1], fGain, fPan);
	}

	float 	fVolumes[2];
	float	fGain;
	float	fPan;
};


#endif // StereoVolumes_H
