/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef StereoVolumes_H
#define StereoVolumes_H


#include <SupportDefs.h>
#include "Log.h"

// In a vanilla non-WAVE_FORMAT_EXTENSIBLE file the 1st channel 
// is defined to be mapped to the left speaker, the 2nd channel 
// to the right speaker

// https://wavefilegem.com/how_wave_files_work.html

#define SV_LEFT		0
#define SV_RIGHT	1

class StereoVolumes {
public:
		StereoVolumes(){ fPan = 0 ; fGain = 1.0f; updateVolumes(); }

	void SetGain(float gain){ fGain = gain; updateVolumes(); }
	void SetPan(float pan)  { fPan  = pan ; updateVolumes(); }
	
	float Gain() { return fGain; }
	float Pan()  { return fPan;  }


	float getVolumeChannel(int8 ch) { return fVolumes[ch]; }
	
	float Left () { return getVolumeChannel(SV_LEFT);  } 
	float Right() { return getVolumeChannel(SV_RIGHT); } 

private:

	void updateVolumes(){				 
			fVolumes[0] = fGain;
			fVolumes[1] = fGain;

		if(fPan > 0) 
			fVolumes[0] *= (1.0f - fPan);
		else
		if(fPan < 0) 
			fVolumes[1] *= (1.0f + fPan);
		
		LogTrace("[StereoVolumes] fVolumes(L,R)[%f,%f] fGain[%f] fPan[%f]", Left(), Right(), fGain, fPan);
	}

	float 	fVolumes[2];
	float	fGain;
	float	fPan;
};


#endif // StereoVolumes_H
