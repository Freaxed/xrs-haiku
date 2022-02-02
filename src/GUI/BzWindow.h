/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _BZWINDOW_H_
#define _BZWINDOW_H_

#include <Window.h>
#include <Message.h>

typedef bool (*RedirectFunc)(BMessage* msg,void* cookies);
	

class BzWindow: public BWindow{

	public:
		BzWindow(BRect frame,const char *title,window_type type,uint32 flags,uint32 workspaces = B_CURRENT_WORKSPACE):
		BWindow(frame,title,type,flags,workspaces),m_func(NULL){}
		
		void	RedirectMessages(RedirectFunc m_fun,void* ck=NULL);
		virtual void MessageReceived(BMessage *msg);
	
	private:
	
		RedirectFunc	m_func;
		void*				cookies;
		
		
};

#endif
