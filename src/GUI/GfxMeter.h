/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#ifndef _GfxMeter_h_
#define _GfxMeter_h_

#include <View.h>
#include "async.h" //?
#include "ValuableManager.h"

class BBitmap;

class GfxMeter:	public BView
{

	typedef	BView  _inherited;
	
	public:				GfxMeter(BRect frame, ValuableID id);		
						~GfxMeter();								
		void	AttachedToWindow();
		void	DetachedFromWindow();
		void 	Draw(BRect r);
		void 	MessageReceived(BMessage *message);
		
		
	private:
	
		float		pixel_l;
		float		pixel_r;
		
		float		width;
		BBitmap*	on;
		BBitmap*	off;
		BRect		r_up;
		BRect		r_bitmap;
		BRect		r_up_off;
		BRect		r_down;
		
		ValuableID			vID;
};
#endif
//-
