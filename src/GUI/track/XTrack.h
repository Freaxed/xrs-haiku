/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _X_TRACK_
#define _X_TRACK_

#include <View.h>
#include <GraphicsDefs.h>
#include <String.h>

#define	CHANGE_NAME		'ttt'


class XTrack : public BView
{
	public:
			XTrack (BRect,const char*);

	 void 	Draw(BRect);	
	 void 	MouseDown(BPoint);
	 void 	MessageReceived(BMessage*);
	 void	AttachedToWindow();
		  void 	Init(BMessage*);
		  void	Select();
		  void	Deselect();
		  bool	isSelected();
		  void	SetName(const char *t);
		  void	SetTarget(BHandler* h){target=h;};	
		  void	SetPadColor(rgb_color col){ rgb_pad=col;};
		  void	SendRenameMessage();
	private:

		BBitmap *pad;
		bool	selected;
	
		BMessage	*msg;
		BHandler	*target;
		BString		name;
		void			_drawPad();
		rgb_color	rgb_pad;
};
#endif

