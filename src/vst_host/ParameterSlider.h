/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_PARAMETER_SLIDER_H_
#define	_PARAMETER_SLIDER_H_

#include 	<StringView.h>
#include	<Slider.h>
#include	"VSTHost.h"		

class ParameterSlider : public BSlider {
public:
				ParameterSlider (BRect frame, VSTParameter*);
	void		 	AttachedToWindow ();
	void			SetValue (int32 v);
	void			LoadParameter ();
	void			LinkController();

private:
	BStringView *	fDisplay;
	VSTParameter*	fParameter;
	bool				fStore;
};

#endif
