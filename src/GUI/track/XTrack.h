/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _X_TRACK_
#define _X_TRACK_

#include <Control.h>
#include <GraphicsDefs.h>
#include <String.h>


class XTrack : public BControl
{
	public:
			XTrack (BRect,const char*);

	void 	Draw(BRect);	
	void 	MouseDown(BPoint);
	void 	MessageReceived(BMessage*);
	void	AttachedToWindow();

	void	SetSelected(bool selected);

	void	SetName(const char *t);
	void	SetPadColor(rgb_color col){ rgb_pad=col;};
	void	SendRenameMessage();
	
	private:
	
		BBitmap *pad;
		bool	selected;	
		BMessage*	fMessage;
		void		_drawPad();
		rgb_color	rgb_pad;
};
#endif

