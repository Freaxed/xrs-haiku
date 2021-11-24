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
#include "IntValuable.h"

#define	NUM_BUSSES	4

class PMixer: public PBus {

	public:
			
			static PMixer* Get(); //get the 'system' mixer
			
					~PMixer();
			PBus*	BusAt(uint pos);			
			void	ResetBuffers();					

	// from the mixer everythings start and
	// everythings end.

	private:
	
		 PMixer();
		//these are the busses:
		PBus 	busses[NUM_BUSSES];
		
};
#endif
