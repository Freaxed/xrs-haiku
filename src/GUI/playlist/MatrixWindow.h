/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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

class PlaylistBox;
class Sequence;

class MatrixWindow:public XrsWindow
{
	 public:
					
			static MatrixWindow*	Get();
					~MatrixWindow();
					
			bool 	QuitRequested();
			void	Reset(Sequence*, int16 notesPerMeasaure);
	
	private:
				 MatrixWindow();
				PlaylistBox*	fBox;
		
};


#endif

//-
