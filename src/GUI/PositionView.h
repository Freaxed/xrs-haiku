/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef PositionView_h_
#define PositionView_h_

#include <View.h>
#include "ValuableManager.h"


class BBitmap;

class PositionView : public BView 
{
	public:
		
					PositionView(BRect,int d);
		 
	virtual void 		Draw(BRect);
	virtual void 		AttachedToWindow();
	virtual void		DetachedFromWindow();
	
	virtual void		MessageReceived(BMessage*);
	
	
	private:
		void	SetTick(int,int,int);
		
		void	SetPos(int);
		void	SetPat(int);
		void	SetBeat(int);
			
			int pos[3];
			int pat[3];
			int beat[2];
			
			
			BBitmap*	digit;
			BRect	BiRect(int d);
			BRect	TRect(int d);
			void	_reset(int*,int pos,int val,int bpos);
			
			ValuableID	vID;
};
#endif

//-
