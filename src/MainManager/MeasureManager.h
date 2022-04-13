/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

/*
	MeasureManager 17-Dec-2003 
	
	-----------------------
	- 2 giugno 2006 (.AR) -
	-----------------------
	Alcune considerazioni, perchè l'Engine deve sapere troppo di questo?
	Non è meglio se lui prosegue imperterrito nel suo lavoro di far suonare una 
	nota alla volta?
	Nel senso ad ogni TICK, gli si passa che pattern, che beat e che tick da suonare.
	Lui cerca e spoppa la nota e la mette tra quelle possibili.
	
	---------	
	Notify by BMessage who graphically looks at him
	and also is the defauld value mantainer of the currentPattern.

*/
#ifndef MeasureManager_h_
#define MeasureManager_h_

#include <Handler.h>
#include <MessageFilter.h>
#include <Message.h>
#include <Application.h>


class Sequence;

class MeasureManager 
{
	public:
	
		static MeasureManager*	Get();
		
		filter_result 	Filter(BMessage *message, BHandler **target);
		
		void			Reset(Sequence*);
		
		int32			GetCurrentPattern(); //the pattern selected!
		void			SetCurrentPattern(int32 pat); //
		
		//Moving the Sequence 
		bool		MoveNextCol();
		int			SubStep();
		int			ResetStep();
		void		SetPatternMode(bool val);
		
		
//private:
		
		bool			GetPatternMode();
//public:
		
		void			SetPosition(int pos){ curPos=0;curSub=0;};

//no ufficial
		
		int			_getCurPos(){ if(!all_patterns) return -1; else return curPos; };
		int			_getCurPat(){ if(all_patterns) return -1; else return curpat; };
				
	private:

					MeasureManager();
		int			curSub;
		int			curPos;	
		
		int32		curpat;
		Sequence*	sequence;
		bool		all_patterns;
		
};

#endif
