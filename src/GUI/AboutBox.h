/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
	 
	private:
	 		BStringView*	fStatusText;
			BBitmap*		fAboutBitmap;
		 	
};
//--
