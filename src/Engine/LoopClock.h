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

				// Fix the names: a beat is not a 'note'
				// the default song as 4 beats each divided in 4 (4 notes)
				// total of 16 notes...
				// maybe we should use: beat as the beat and tick as the current misused beat..

	public:
				LoopClock():fLoopEnable(true),fMaxBeat (0) { }
		
		 void	Tick()
		 		{
					if(beat == fMaxBeat + 1)
					{
						if (fLoopEnable)
							beat = 0;		 		
						else
							return;			 
					}

					uint64	time = Time();
					NotifyTickedHigh(time);

					tick++;
					if(tick == fDefaultResolution) { beat++; tick = 0; }					
				}

		void	SetLoopEnable(bool enable) 
				{
					 beat = 0;
					 fLoopEnable = enable;
				}

		void	SetMaxBeat(uint32 maxBeat) { fMaxBeat = maxBeat; } //still WRONG but needs a complete refactor.
	private:
		
				uint32	fMaxBeat;	// how many beats on the song ? (WRONG! see above)
				bool	fLoopEnable;
		

};
#endif
//--

