/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include	"ParameterSlider.h"
#include	<stdio.h>

void clean_string (char* d)
{	// removes leading & ending spaces & tabs
	char* f = d;
	char* s = d;
	while (*s == ' ' || *s == '\t')
		s++;
	while (*s)
		*d++ = *s++;
	d--;
	while (d >= f && (*d == ' ' || *d == '\t'))
		d--;
	d++;
	*d = 0;
}

// Generic Parameter
ParameterSlider::ParameterSlider (BRect frame, AEffect * effect, int index)
	:BSlider (frame, B_EMPTY_STRING, "", NULL, 0, 10000), fEffect (effect), fIndex (index), fStore (false)
{
	SetFontSize(10);
	frame.OffsetTo (B_ORIGIN);
	frame.left = (frame.right + frame.left) / 2;
	frame.top+=5;
	frame.bottom = frame.top + 11;
	memset(fUnit,'\0',64);
	fEffect->dispatcher (fEffect, effGetParamLabel, fIndex, 0, fUnit, 0.f);
	clean_string (fUnit);
	fDisplay = new BStringView (frame, NULL, fUnit);
	fDisplay->SetFontSize(10);
	AddChild (fDisplay);
	fDisplay->SetAlignment (B_ALIGN_RIGHT);
}
					
void ParameterSlider::SetValue (int32 v)
{
	BSlider::SetValue (v);
	char	display[64];
	float	value = float (v) / 10000.f;
	if (fStore)
		fEffect->setParameter (fEffect, fIndex, value);
	fEffect->dispatcher (fEffect, effGetParamDisplay, fIndex, 0, display, 0.f);
	clean_string (display);
	char	complete[128];
	sprintf (complete, "%s %s", display, fUnit);
	if (strcmp (complete, fDisplay->Text ()) != 0)
		fDisplay->SetText (complete);
}

void ParameterSlider::LinkController()
{

}
void ParameterSlider::AttachedToWindow ()
{
	BSlider::AttachedToWindow ();
	LoadParameter ();
	char	name[64];
	fEffect->dispatcher (fEffect, effGetParamName, fIndex, 0, name, 0.f);
	clean_string (name);
	SetLabel (name);
	if(fIndex % 2 )  {
						SetViewColor(tint_color(ViewColor(),B_DARKEN_1_TINT));
						fDisplay->SetViewColor(255,0,0);
					}
}

void ParameterSlider::LoadParameter ()
{
	fStore = false;
	SetValue (int32 (fEffect->getParameter (fEffect, fIndex) * 10000.f));
	fStore = true;
}
