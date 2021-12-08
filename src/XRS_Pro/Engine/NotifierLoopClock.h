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
#include	"ValuableManager.h"

#define P0	"time.position.fulltick.substep"
#define P1  "time.position.fulltick.pattern"
#define P2  "time.position.fulltick.position"

class NotifierLoopClock : public LoopClock {

	public:
				NotifierLoopClock(){}
				
		
		 void	TickAndNotify(bigtime_t delay){
					Notify(beat, 0, 0, delay);
					Tick();
				}
		
		void	ResetAndNotify(bigtime_t delay = 0){
					Reset();
					Notify(-1,-1,-1,delay);					
				}	
				
		void	SendValue(ValuableID vID, int32 value, bigtime_t fixme) {
					ValuableManager::Get()->UpdateValue(vID, value);
				}
						
	private:
	
		void	Notify(int32 beat,int32 b, int32 c, bigtime_t){
					ValuableManager::Get()->UpdateValue(P0, beat);
					ValuableManager::Get()->UpdateValue(P1, b);
					ValuableManager::Get()->UpdateValue(P2, c);
				}

};
#endif
//--

