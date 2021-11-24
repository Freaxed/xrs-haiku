/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Window.h"

class BStringView;

class AboutBox : public BWindow
{
	public:
	 		AboutBox(bool);
	 	   ~AboutBox();
	 	
	 	void 	setText(const char*);
		void 	WindowActivated(bool);
		void	MessageReceived(BMessage*);
	 
	private:
	 		//TODO: fix these ugly names:
	 		
	 		BStringView *text;
	 		BBitmap*	shot;
			BView*		w;
			BPoint 		centered_point;
			bool		big_mode;
		 	
};
//--
