/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef NotifierLoopClock_H_
#define NotifierLoopClock_H_

#include 	"LoopClock.h"
#include	"ValuableController.h"

class NotifierLoopClock : public LoopClock, public ValuableController {

	public:
				NotifierLoopClock(){
					SetValuableID("time.position.fulltick");
				}
				
		
		 void	TickAndNotify(bigtime_t delay){
					Notify(beat,0,0,delay);
					Tick();
				}
		
		void	ResetAndNotify(bigtime_t delay = 0){
					Reset();
					Notify(-1,-1,-1,delay);					
				}			
	private:
	
		void	Notify(int beat,int b, int c,bigtime_t delay){
					SendValue(0,beat,delay);
					SendValue(1,b,delay);
					SendValue(2,c,delay);
				}
};
#endif
//--

