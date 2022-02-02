/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef LoopClock_H_
#define LoopClock_H_

#include "Clock.h"

class LoopClock : public Clock {

	public:
				LoopClock(){ maxbeat = 15; }
		
		 void	Tick()
		 		{ 
		 						 				
		 				uint64	time=Time();
						NotifyTickedHigh(time);
												
						tick++;
						if(tick == fDefaultResolution) { beat++; tick = 0; }
						if(beat == maxbeat + 1 ) beat = 0;
				}
					
	private:
		
		long	maxbeat;
		

};
#endif
//--

