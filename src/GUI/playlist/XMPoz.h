/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _XMPoz_
#define _XMPoz_

#include <View.h>
#include "ValuableManager.h"
class Sequence;

class XMPoz: public BView
{
	public:
			XMPoz(BRect);
			
		 void 	Draw(BRect);
		 void   	DrawAfterChildren(BRect r);
		 void	MouseDown(BPoint p);
		 void	MouseUp(BPoint p);
		 void	MouseMoved(BPoint point,uint32 transit,const BMessage *message);
		 void	MessageReceived(BMessage*);
		 void	AttachedToWindow();
		 void	DetachedFromWindow();

			
			
			 void		Reset(Sequence*, int16 notesPerMeasaure);
			 void		setMaxs(int,int);		
				
	private:
			void		setPosition(int,int);
			void		setPositionPar(int);
			
			Sequence*	sequence;
			int 			curPat;
			int			loop_start,loop_end;
			void			MoveMarker(int i,int block,bool set=true);
			int			MarkerPosition(int i);
			bool			tracking[2];
						
			void			_drawCell(int i);
			void			_drawTick(int i);
			
			float factor;
			
			//accumulate (don't like)

};			

#endif
