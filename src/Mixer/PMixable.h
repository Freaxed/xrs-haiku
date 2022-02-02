/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */
/*
	PMixable
	
	A child from PMixable and Pannable.
*/
#ifndef _PMixable_H_
#define _PMixable_H_

#include "PNode.h"
#include "StereoVolumes.h"
#include <SupportKit.h>
#include "Valuable.h"

class PMixable: public PNode , public StereoVolumes {

	public:

					 PMixable();
			virtual ~PMixable();
			
			// AddBuffer: add to current buffer (at spaiz) size frame from 'from'
			
			virtual void 			MixBuffer(float** from,size_t frames,int32 spiaz);
			
			// MeltBuffer: add but with factors (return MAX arrays..lastop )
			// void			MeltBuffer(float**to,size_t size,float *factors);
			
		
			void		SetVID(ValuableID val){ vID=val; }
			ValuableID	GetVID(){ return vID; }
			
			float	GetLastMaxValue(int channel);
	
			float**			Buffer(){ return stream;}
			void			ClearBuffer(int32 size = MAX_FRAMES*FRAMESIZE);
	protected:		
			float 			*stream[NUM_CHANNELS];
			float			lastop[NUM_CHANNELS];
	private:
	
			ValuableID	vID;
			float		fMaxValue[NUM_CHANNELS];
			
};
#endif
