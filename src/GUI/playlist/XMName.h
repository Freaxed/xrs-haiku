/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _XMName_h
#define _XMName_h

#include <View.h>

class Sequence;

class XMName: public BView
{
	public:
					XMName(BRect);
			  void	Reset(Sequence*);
					
		 void 	Draw(BRect);
		 void 	MouseDown(BPoint);
		 void	AttachedToWindow();	
		 void	DetachedFromWindow();
		 void	MessageReceived(BMessage* msg);
			
				 
			 void		_MessForRect(int num);			
	private:
			void	Select(int);
		
			Sequence*	sequence;
			
			int 	sel;
			int  y_count;
			bool	key_rel;
			
			void	_drawName(int pos);
};			

#endif
