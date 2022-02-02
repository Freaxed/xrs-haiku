/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef NotifierLoopClock_H_
#define NotifierLoopClock_H_

#include 	"LoopClock.h"
#include	"ValuableManager.h"
#include	"CommonValuableID.h"

//Fix me..
#define P0	VID_TEMPO_BEAT
#define P1  VID_TEMPO_PATTERN
#define P2  VID_TEMPO_MEASURE

class NotifierLoopClock : public LoopClock, public  Tickable {

	public:
				
				NotifierLoopClock(){
					//AddTickable(this);	
				}
				
				
			//starting events..
			void	TickedHigh(uint64 time,int16 beat,int16 tick) {
				printf("time %ld - beat %d - tick %d\n");
			}
	
			//stopping events..
			void	TickedLow(uint64 time,int16 beat,int16 tick) {}
	
	
		void	ResetAndNotify(bigtime_t fixme = 0){
					Reset();
					Notify(-1,-1,-1, 0);					
				}	
				
		void	SendValue(ValuableID vID, int32 value, bigtime_t fixme = 0) {
					ValuableManager::Get()->UpdateValue(vID, value);
				}
						
	private:
	
		void	Notify(int32 beat,int32 b, int32 c, bigtime_t fixme){
					ValuableManager::Get()->UpdateValue(P0, beat);
					ValuableManager::Get()->UpdateValue(P1, b);
					ValuableManager::Get()->UpdateValue(P2, c);
				}

};
#endif
//--

