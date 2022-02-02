/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TRACKEND_H_
#define _TRACKEND_H_

#include <View.h>
#include <Button.h>

class BPictureButton;

class TrackEnd: public BView
{
	public:
			     			TrackEnd(BRect r, char* name);
	 		virtual 	void 		MessageReceived(BMessage*);
	 		virtual 	void		Draw(BRect r);
	 		virtual 	void 		AttachedToWindow();
	 				bool		IsExpanded(){ return expanded;}
	 		virtual	void		Expand();
	 		virtual	void		UnExpand();
	 		virtual	float		getExpansionSize() { return 15.0;};
	 		virtual	void		Refresh(){};
	 		
	private:
			bool 				expanded;
			BPictureButton *brez;
			
};
#endif

//--
