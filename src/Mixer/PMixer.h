/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
			uint8	CountBusses() { return 	MIXERLINES_COUNT;}	

			void	ValuableChanged(BMessage* msg); //Mixer related, triggered by JuiceEngine!

			void	SaveSettings(BMessage& msg);
			void	LoadSettings(BMessage& msg);

	// from the mixer everythings start and
	// everythings end.

	private:
	
		 PMixer();
		//these are the busses:
		PBus 	busses[MIXERLINES_COUNT];
		
};
#endif
