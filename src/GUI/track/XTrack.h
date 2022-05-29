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
	void 	SetID(int16 id);

	void	SetSelected(bool selected);

	void	SetName(const char *t);
	void	SetTarget(BHandler* h){target=h;};	
	void	SetPadColor(rgb_color col){ rgb_pad=col;};
	void	SendRenameMessage();
	
	private:

		BBitmap *pad;
		bool	selected;
	
		BMessage	fMessage;
		BHandler	*target;
		BString		name;
		void			_drawPad();
		rgb_color	rgb_pad;
};
#endif

