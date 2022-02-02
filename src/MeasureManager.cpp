/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "MeasureManager.h"
#include "Sequence.h"
#include "ValuableManager.h"

#include <stdio.h>


MeasureManager::MeasureManager()
	:BMessageFilter(B_PROGRAMMED_DELIVERY,B_LOCAL_SOURCE,SETPAT),
	curpat(0),sequence(NULL)
{
	ValuableManager::Get()->RegisterValuable("measure.allpatters", 0);
	
	curPos=0;
}

MeasureManager*
MeasureManager::Get()
{
	static	MeasureManager*	instance = NULL;
	if (instance == NULL) 
			instance = new MeasureManager();
	return instance;
}

void
MeasureManager::Reset(Sequence* s)
{ 
	sequence = s; 
	curpat = 0; 
	all_patterns = s->all;
}

int
MeasureManager::GetCurrentPattern()
{ 
	return curpat;
}

void
MeasureManager::RegisterMe(BHandler* lop)
{
	if(!lop) 
		return;
	hands.Add(lop);
}

int			
MeasureManager::ResetStep()
{ 
	curSub=0;
	
 	if(all_patterns)
 	  return SubStep();
	 else
	   return GetCurrentPattern();
}

int
MeasureManager::SubStep()
{
	if(!all_patterns) 
		return -1;
	
	int r = sequence->ItemAt(curPos,curSub);	
	
	if (curSub>=sequence->getMaxPat()) 
	{
		curSub=0; 
		return -1;
	}
	
	for(int p=curSub; p<sequence->getMaxPat(); p++)
	{
		r = sequence->ItemAt(curPos,curSub);
		curSub++;
		if (r != -1)   
			return r;	
	}
		
	curSub=0;
	return -1;
}

bool
MeasureManager::MoveNextCol()
{
	if(!all_patterns) 
		return true;
	
	bool ret=false;
	
	if(sequence->loop_enable && curPos==sequence->loop_points[1])
		curPos=sequence->loop_points[0];
	else	
		curPos++;
		
	ResetStep();	//reset dei substep
		
	if (curPos>sequence->getMaxSeq()) 
	{
			curPos=0;
			ret=true;
	}
	
	return ret;
}
void
MeasureManager::SetPatternMode(bool allpat)
{
	all_patterns=allpat;
	sequence->all=allpat;
}
bool
MeasureManager::GetPatternMode()
{
	return sequence->all;
}
void
MeasureManager::SetCurrentPattern(int newpat)
{ 
	curpat=newpat;
	BMessage *message = new BMessage(SETPAT);
	
	for(int i=0; i<hands.Count(); i++)
	{
		(hands[i]->Looper())->PostMessage(message,hands[i]);
	}
}

filter_result
MeasureManager::Filter(BMessage *message, BHandler **target)
{
	curpat = message->FindInt32("be:value");
	message->RemoveName("be:value"); //bastard! (?)
	
	for(int i=0; i<hands.Count(); i++)
	{
		(hands[i]->Looper())->PostMessage(message,hands[i]);
	}
	return B_SKIP_MESSAGE;
	
}
