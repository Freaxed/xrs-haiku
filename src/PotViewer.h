/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef PotViewer_H_
#define PotViewer_H_

#include <Window.h>
#include <StringView.h>
#include "APot.h"


class PotViewer: public BWindow {

	public:
				PotViewer();
		void	InitShow(APot*,float distance = 0);
		void	InitHide();
		void	SetValue(long value);
		
		void	AllAttachted();

	private:
	
		BStringView*	fText;
};

//extern PotViewer* potviewer;

#endif

//------

