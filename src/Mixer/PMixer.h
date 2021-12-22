/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

/*
	PMixer:	A BUS without a next. (and more..)
*/
#ifndef _PMixer_H_
#define _PMixer_H_

#include "PBus.h"
#include "PEffector.h"
#include "CommonValuableID.h"

class PMixer {

	public:
			
			static PMixer* Get(); //get the 'system' mixer
			
					~PMixer();
					
			PBus*	BusAt(uint8 pos);
			PBus*	GetMain();			
			void	ResetBuffers();					

	// from the mixer everythings start and
	// everythings end.

	private:
	
		 PMixer();
		//these are the busses:
		PBus 	busses[MIXERLINES_COUNT];
		
};
#endif
