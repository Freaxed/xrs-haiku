/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef Clock_H_
#define Clock_H_

#include "VectorNew.h"
#include "Tickable.h"

#include "Resolution.h"

class Clock {

	public:
						Clock(){ Reset(); }
		
		virtual			~Clock(){};
		
		virtual	void	Tick() = 0;
		
		void	AddTickable(Tickable* t){ fLTick.Add(t); }
	
		void	Reset(){ beat = tick = 0; }
		
		uint64	Time(){ return beat*fDefaultResolution+tick;}
		
		long	Resolution(){ return fDefaultResolution; }
		
		//deprecated?
		// void	NotifyTickedLow(uint64	time,int16 beat,int16 tick){
		// 	for(int i=0;i<fLTick.Count();i++)
		// 		fLTick[i]->TickedLow(time,beat,tick);
							
		// }
		
		void	NotifyTickedHigh(uint64	time){
			for(int i=0;i<fLTick.Count();i++)
				fLTick[i]->TickedHigh(time, beat, tick);
		}
				
	protected:
		int16	beat;
		int16	tick;
	
	private:	
		VectorNew<Tickable*>	fLTick;		

};
#endif
//--

