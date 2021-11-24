/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef MATRIX_WINDOW_H
#define MATRIX_WINDOW_H
 
#include "XrsWindow.h"
#include "GlobalDef.h"
#include	<CheckBox.h>

class XMatrix;
class XMName;
class Sequence;
class XMPoz;

class MatrixWindow:public XrsWindow
{
	 public:
					
			static MatrixWindow*	Get();
					~MatrixWindow();
					
			 void MessageReceived(BMessage*);
			 bool QuitRequested();
			 void FrameResized(float,float);
				
			
				void	Reset(Sequence*);
				//void	setPosition(int);
	
	private:
				 MatrixWindow();
				BCheckBox	*en;
				XMatrix 	*the_m;
				XMName		*the_n;
				BScrollView *scroll;
				BScrollBar	*scroll_bar;
				Sequence	*seq;
				int 			y_count;
				XMPoz *tt;
		
};


//deprecated extern MatrixWindow*	mw;

#endif

//-
