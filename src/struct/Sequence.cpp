/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Sequence.h"
#include <stdio.h>
#include <String.h>
#include <List.h>

/*
	MaxPat= massimo per righe
	MaxSeq= massimo per colonne.
*/

Sequence::Sequence()
{
	MaxSeq=-1;
	MaxPat=0;
	matrix2=NULL;
	loop_points[0]=loop_points[1]=0;
	loop_enable=false;
	all=false;
}
void
Sequence::AddMeasure(const char* name){

	names.AddItem((void*)(new BString(name) ));
	MaxPat++;

}
BString*
Sequence::GetMeasureName(int pos){

	if(pos<0 || pos >= names.CountItems()) return NULL;
	return (BString*)(names.ItemAt(pos));
}
void
Sequence::SetMeasureName(const char* s,int pos)
{
	if(pos<0 || pos >= names.CountItems()) return;
	 ((BString*)(names.ItemAt(pos)))->SetTo(s);
}


int
Sequence::Reset()
{
	if(matrix2!=NULL) delete matrix2;
	matrix2=new BTimedEventQueue();

	for(int i=0;i<names.CountItems();i++)
	{
		BString* s=(BString*)names.ItemAt(i);
		if(s!=NULL) delete s;
		names.RemoveItem(i);
	}
	MaxSeq=0;
	return 0;
					
}

void
Sequence::RemoveMeasure(int pos)
{
	
	int maxs=getMaxSeq()+1;
	const media_timed_event *find;
	
	for(int x=0;x<maxs;x++)
	{
		find=matrix2->FindFirstMatch(x*10000+pos,BTimedEventQueue::B_AT_TIME,true);
		if(find) matrix2->RemoveEvent(find);
		
		/* qua devono scalare tutti :( */
		
		for(int j=pos+1;j<getMaxPat();j++){
			find = matrix2->FindFirstMatch(x*10000+j,BTimedEventQueue::B_AT_TIME,true);
			if(find){
					matrix2->RemoveEvent(find);
					media_timed_event nmev(x*10000+j-1,BTimedEventQueue::B_USER_EVENT);
					matrix2->AddEvent(nmev);					
			}		
		}		
	}

	BString* s=(BString*)names.ItemAt(pos);
	if(s!=NULL) delete s;
	names.RemoveItem(pos);

}


void	Sequence::setMaxSeq(int d) { MaxSeq=d; }
int		Sequence::getMaxPat() { return MaxPat; }
void	Sequence::setMaxPat(int d) { MaxPat=d; }

//XRS2 (ver 1.3) new implementation::

int
Sequence::getMaxSeq() {

	bigtime_t max=matrix2->LastEventTime();
	if(max==B_INFINITE_TIMEOUT) return 0;
	return max/10000;
}

void
Sequence::setItemAt(int col, int row, bool c)
{
	const media_timed_event *mev = matrix2->FindFirstMatch(col*10000+row,BTimedEventQueue::B_AT_TIME,true);
	if(mev!=NULL) 
	{
		matrix2->RemoveEvent(mev);
		return;
	}
	media_timed_event nmev(col*10000 + row,BTimedEventQueue::B_USER_EVENT);
	matrix2->AddEvent(nmev);
			
}

int
Sequence::ItemAt(int col, int row)
{	
	const media_timed_event *mev = matrix2->FindFirstMatch(col*10000+row,BTimedEventQueue::B_AT_TIME,true);
	
	if ( mev == NULL) 
		return -1;

	return row;
}

#include <assert.h>

void 
Sequence::LoadSettings(BMessage& playlist)
{
	assert(names.CountItems() == 1);
	
	BMessage	measures;
	playlist.FindMessage("Measures", &measures);
	int maxPat = measures.GetInt16("NumMeasures", 1);
	
	int i=0;
	BMessage measure;
	while(measures.FindMessage("Measure", i, &measure) == B_OK)
	{
		if (i == 0){
			SetMeasureName(measure.GetString("Name", "?"), 0);
		} else {
			AddMeasure(measure.GetString("Name", "?"));
		}
		
		//TODO load the patterns..
		int col = 0;
		int16 val = -1;
		while(measure.FindInt16("Pattern", col++, &val) == B_OK)
		{
			setItemAt(val, i, true);
		}
		
		i++;
	}
	
	loop_enable = playlist.GetBool("Enable", false);
	loop_points[0] = playlist.GetInt16("Point", 0, 0);
	loop_points[1] = playlist.GetInt16("Point", 1, 0);

}

void 
Sequence::SaveSettings(BMessage& playlist)
{
	BMessage	measures;
	measures.AddInt16("NumMeasures", MaxPat);
	for (int i=0; i<MaxPat; i++) {
		BMessage measure;
		BString* name = GetMeasureName(i);
		assert(name);
		measure.AddString("Name", name->String());
		
		for(int p=0; p < getMaxSeq() + 1; p++) //WHY +1?? (last + 1 = count)
		{
			if ( ItemAt(p, i) >= 0 )
				measure.AddInt16("Pattern", p);
		}
		
		measures.AddMessage("Measure", &measure);
	}
	
	playlist.AddMessage("Measures", &measures);	
	playlist.AddBool("Enable", loop_enable);
	playlist.AddInt16("Point", loop_points[0]);
	playlist.AddInt16("Point", loop_points[1]);
	playlist.AddInt16("MaxSeq", getMaxSeq());;
}
