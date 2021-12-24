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

BRect& shorter_rect(BRect& rect) {
	rect.bottom -= 30;
	return rect;
}
// Generic Parameter
ParameterSlider::ParameterSlider (BRect frame, VSTParameter* para)
	:BSlider (shorter_rect(frame), para->Name(), "", NULL, 0, FACTOR_I), fParameter (para), fStore (false)
{
	SetOrientation(B_VERTICAL);
	SetFontSize(10);	
	BString label(fParameter->Name());
	if (strlen(fParameter->Unit()) > 0)
		label << " (" << fParameter->Unit() << ")";
		
	SetLabel (label.String());
	SetLimitLabels(fParameter->MinimumValue(), fParameter->MaximumValue());
	//BRect rect (0, frame.Height(), frame.Width(), frame.Height() + 30);
	frame.top    = frame.bottom + 1;
	frame.bottom = frame.top    + 30;
	fDisplay = new BStringView (frame, NULL, fParameter->Unit());
	fDisplay->SetFontSize(10);
	fDisplay->SetAlignment (B_ALIGN_CENTER);
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
	Parent()->AddChild(fDisplay);
	float w,h;
	GetPreferredSize(&w, &h);
	fDisplay->ResizeTo(w, 30);
	LoadParameter ();

	if(fParameter->Index() % 2 )  {
	   //SetViewColor();
	   fDisplay->SetViewColor(tint_color(ViewColor(),B_DARKEN_1_TINT));
	}
}

void ParameterSlider::LoadParameter ()
{
	fStore = false;
	fParameter->Value();
	SetValue ((int32)(fParameter->Value() * FACTOR_F));
	fStore = true;
}
