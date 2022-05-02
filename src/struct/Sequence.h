/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <TimedEventQueue.h>
#include <StringList.h>

class BString;

class Sequence
{
	public:
	
					Sequence();
			
		int			Reset();
		int			getMaxSeq();
		int			getMaxPat();
		void		setMaxPat(int);
		void		setMaxSeq(int);
		void		RemoveMeasure(int pos);
		void		AddMeasure(const char*);
		
		BString		GetMeasureName(int);
		void		SetMeasureName(const char*,int);
		void		setItemAt(int col,int raw,bool c=false);
		int			ItemAt(int col,int raw=0);
		
		int		loop_points[2];
		bool	loop_enable;
		bool 	all;

		void LoadSettings(BMessage& msg);
		void SaveSettings(BMessage& msg);
		
	private:

				
		BTimedEventQueue*	matrix2;
		BStringList			fNames;
		int					MaxSeq,MaxPat;

};

#endif
