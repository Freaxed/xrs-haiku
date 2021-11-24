/*
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#ifndef _XRSWINDOW_H_
#define _XRSWINDOW_H_

#include <Window.h>
#include <Message.h>
#include <String.h>

class XrsWindow : public BWindow {

	public:
		XrsWindow(BRect frame,const char *title,window_type type,uint32 flags,uint32 workspaces = B_CURRENT_WORKSPACE):
			BWindow(frame,title,type,flags,workspaces){ fFrame = frame;}

		XrsWindow(BRect r,const char *name,window_look look, window_feel feel,uint32 flags, uint32 workspaces=B_CURRENT_WORKSPACE):
			BWindow(r,name,look,feel,workspaces){ fFrame = r;}
		
		void	SaveConfig();
		void	LoadConfig(BRect *frame = NULL);
		void	SetName(const char* name);
	
	private:
		BString	fName;
		BRect	fFrame;
};

#endif
