/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef PotViewer_H_
#define PotViewer_H_

#include <Window.h>
#include <StringView.h>


class PotViewer: public BWindow {


				PotViewer();
		
public:
		static	PotViewer*	Get();
		
		void	InitShow(BView*,float distance = 0);
		void	InitHide();
		void	SetValue(int32 value);
		void	SetValue(BString value);
		
		void	AllAttachted();

private:
		void	SetText(BString text);
		BStringView*	fText;
};

#endif

//------

