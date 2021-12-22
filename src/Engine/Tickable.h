/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef Tickable_H_
#define Tickable_H_

class Tickable {
	public:
	
	//starting events..
	virtual	void	TickedHigh(uint64 time,int16 beat,int16 tick) = 0;
	
	//stopping events..
	virtual	void	TickedLow(uint64 time,int16 beat,int16 tick) = 0;
};
#endif

//--

