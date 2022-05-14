/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
						XMatrix(BRect, BView*, XMPoz*);
			
				void 	Draw(BRect);
				void 	MouseDown(BPoint);
				void	AttachedToWindow();	
				void	ScrollTo(BPoint);
				void	MessageReceived(BMessage *m);
				void	MouseMoved(BPoint where, uint32 code, const BMessage *dragDropMsg);
			
				void	DetachedFromWindow();
			
				void	Reset(Sequence*);
				void	AddItem(int,int);
			 
	private:
			
			Sequence*	sequence;
			
			void		Select(int);
			int 		curPat;
			int			curSub;
			int 		sel;
			int 		y_count;
			BView*		the_n;
			XMPoz*		the_s;
			int 		MaxPat;
			bool		key_rel;

			void  		_drawCell(int x,int y);
};			

#endif
