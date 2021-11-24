/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef Clock_H_
#define Clock_H_

#include "Vector.h"
#include "Tickable.h"

#include "Resolution.h"

class Clock {

	public:
						Clock(){ Reset(); }
		
		virtual			~Clock(){};
		
		virtual void	Tick(){}
		
		void	AddTickable(Tickable* t){ fLTick.Add(t); }
	
		void	Reset(){ beat=tick=0; }
		
		uint64	Time(){ return beat*fDefaultResolution+tick;}
		
		long	Resolution(){ return fDefaultResolution; }
		
		void	NotifyTickedLow(uint64	time,long beat,long tick){
			for(int i=0;i<fLTick.Count();i++)
				fLTick[i]->TickedLow(time,beat,tick);
							
		}
		
		void	NotifyTickedHigh(uint64	time){
			for(int i=0;i<fLTick.Count();i++)
				fLTick[i]->TickedHigh(time,beat,tick);
		}
				
	protected:
		long	beat;
		long	tick;
	
	private:	
		Vector<Tickable*>	fLTick;		

};
#endif
//--

