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
		 void	MouseDown(BPoint p);
		 void	MouseUp(BPoint p);
		 void	MouseMoved(BPoint point,uint32 transit,const BMessage *message);
		 void	MessageReceived(BMessage*);
		 void	AttachedToWindow();
		 void	DetachedFromWindow();

			
			
		 void	Reset(Sequence*, int16 notesPerMeasaure);
				
	private:
			void		setPosition(int,int);
			void		setPositionPar(int);
			
			Sequence*	sequence;
			int 		curPat;
			
			void		MoveMarker(int i,int block);
			int			MarkerPosition(int i);

			int8		fTracking;
						
			void		_drawCell(int i);
			void		_drawTick(int i);
			
			float 		factor;
			BRect 		marker[2];
			int16		fMarkerPosition[2];
			void		UpdateMarkerPosition(int who, int position);
			
			//accumulate (don't like)

};			

#endif
