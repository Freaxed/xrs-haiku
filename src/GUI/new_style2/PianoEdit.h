/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _PIANO_EDIT_H
#define _PIANO_EDIT_H

#include <View.h>
#include "Pattern.h"

#define	MOVESCROLL	'mosc'
#define	NOTE		'note'
#define	SLIDE		'slid'
#define	NOTESIZE	10.0f
#define	TOTALSIZE	120.0f	// piano Height


class PianoEdit : public BView
{
	public:
		 	PianoEdit(BRect r);
	 void 	Reset(Pattern* p);
	 void 	Draw(BRect);
	 void 	MouseDown(BPoint point);
	 void 	AttachedToWindow(){ SetViewColor(B_TRANSPARENT_COLOR);  };
	 void	MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
	 void	ScrollTo(BPoint);
	 void	_drawNote(int i,int j);

	 bool	IsSharp(int v);
	 
	private:
		Pattern*		fPattern;
		BBitmap*		fPianoBitmap;
};

#endif
