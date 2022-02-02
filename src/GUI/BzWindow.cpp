/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "BzWindow.h"


void
BzWindow::RedirectMessages(RedirectFunc m_f,void* ck){

	m_func=m_f;
	cookies=ck;
}

void 
BzWindow::MessageReceived(BMessage *msg){

	bool b;
	if(m_func!=NULL)
		b=m_func(msg,cookies);
		
	if(b) BWindow::MessageReceived(msg);
}
