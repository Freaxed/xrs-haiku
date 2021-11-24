/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _X_MATRIX_
#define _X_MATRIX_

#include <View.h>

class Sequence;
class XMPoz;


class XMatrix: public BView
{
	public:
			XMatrix(BRect,BView*,XMPoz*);
			
		virtual void 	Draw(BRect);
		virtual void 	MouseDown(BPoint);
		virtual void	AttachedToWindow();	
		virtual void	ScrollTo(BPoint);
		virtual void	MessageReceived(BMessage *m);
		virtual void	MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
			
			
			
			 void		Reset(Sequence*);
			 void		AddItem(int,int);
			 int		getMaxPat();
			 //void		setPosition(int);
			 void		setMaxs(int,int);
			 
			 //int		substep();
			 //bool		moveNextCol();
			 //void		resetStep();	
	private:
			
			Sequence*	sequence;
			
			void		Select(int);
			int 		curPat;
			int		curSub;
			int 		sel;
			int 		y_count;
			BView*	the_n;
			XMPoz*	the_s;
			int 		MaxPat;
			bool		key_rel;
			BBitmap	*morbido;
			void  _drawBall(int x,int y);
			
		
};			

#endif
