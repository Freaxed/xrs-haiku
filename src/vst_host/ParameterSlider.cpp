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

#define		FACTOR_I	((int32)1000)
#define		FACTOR_F	((float)1000.0f)

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
ParameterSlider::ParameterSlider (BRect frame, VSTParameter* para)
	:BSlider (frame, para->Name(), "", NULL, 0, FACTOR_I), fParameter (para), fStore (false)
{
	SetFontSize(10);
	frame.OffsetTo (B_ORIGIN);
	frame.left = (frame.right + frame.left) / 2;
	frame.top += 15;
	frame.bottom = frame.top + 110;//!!!
	

	fDisplay = new BStringView (frame, NULL, fParameter->Unit());
	fDisplay->SetFontSize(10);
	AddChild (fDisplay);
	fDisplay->SetAlignment (B_ALIGN_RIGHT);
}
					
void ParameterSlider::SetValue (int32 v)
{
	BSlider::SetValue (v);
	float	value = float (v) / FACTOR_F;
	if (fStore)
		fParameter->SetValue (value);

	char	complete[128];
	sprintf (complete, "%s %s", fParameter->CurrentValue(), fParameter->Unit());
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
	SetLabel (fParameter->Name());
	if(fParameter->Index() % 2 )  {
	   SetViewColor(tint_color(ViewColor(),B_DARKEN_1_TINT));
	   fDisplay->SetViewColor(255,0,0);
	}
}

void ParameterSlider::LoadParameter ()
{
	fStore = false;
	fParameter->Value();
	SetValue ((int32)(fParameter->Value() * FACTOR_F));
	fStore = true;
}
