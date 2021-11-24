/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_PARAMETER_SLIDER_H_
#define	_PARAMETER_SLIDER_H_

#include 	<StringView.h>
#include	<Slider.h>

#ifndef __audioeffectx__
#include "audioeffectx.h"
#endif		

class ParameterSlider : public BSlider {
public:
				ParameterSlider (BRect frame, AEffect * effect, int index);
	void		 	AttachedToWindow ();
	void			SetValue (int32 v);
	void			LoadParameter ();
	void			LinkController();

private:
	BStringView *	fDisplay;
	AEffect *		fEffect;
	int				fIndex;
	char				fUnit[64];
	bool				fStore;
};

#endif
