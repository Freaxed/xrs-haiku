/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <View.h>

class TickView : public BView
{
	public:
		
					TickView(BRect,int d);
		  void 		SetTick(int,int,int);
		  void		SetNumberNotes(int);
	virtual void 		Draw(BRect);
	virtual void 		AttachedToWindow();
	
	virtual void		MessageReceived(BMessage*);
	
	private:
			float	xinc,space,space2;
			int tick,num_notes;
			BRect	TRect(int d);
};

//-
