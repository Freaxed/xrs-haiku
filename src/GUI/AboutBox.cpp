/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "AboutBox.h"
#include "Xed_Utils.h"
#include "version.h"
#include "locale.h"
#include "commit.h"
#include <Alert.h>
#include <Box.h>
#include <Window.h>
#include <Font.h>
#include "Colors.h"

#define PREFS_H 300
#define PREFS_L 300


AboutBox::AboutBox(bool big_mode):BWindow( BRect( 0, 0, PREFS_L, PREFS_H), 
                                           "About XRS", 
										   B_TITLED_WINDOW_LOOK, 
										   B_MODAL_APP_WINDOW_FEEL, 
										   B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE)
{
	fStatusText = NULL;
	float larg = PREFS_L;

	MoveTo(-2*larg,-2*PREFS_H);
	BPoint centered_point = BAlert::AlertPosition(larg,PREFS_H);

	if(!big_mode) 
		SetLook(B_NO_BORDER_WINDOW_LOOK);
	
	BView *sporca=new BView(BRect(0,0,10,10),"",B_FOLLOW_NONE,B_WILL_DRAW);
	AddChild(sporca);
	
	XUtils::ResetList(sporca);
	RemoveChild(sporca);


	fAboutBitmap = XUtils::CreateBootBitmap(Bounds());

	
	BRect r(Bounds());
	BView* w;
	AddChild(w = new BView(r, "LogoView", B_FOLLOW_NONE, B_WILL_DRAW));
	
	
	
	float k   = big_mode ? 150 : 20;
	r.top 	  = r.bottom + 1;
	r.bottom += k;

	ResizeBy(0, k-1);
	
	BBox* but = new BBox(r, "UnderTheLogo", B_FOLLOW_ALL, B_WILL_DRAW);
	
	if(big_mode)
	{
		BRect x(0, 0, 299 - B_V_SCROLL_BAR_WIDTH , 149);
		BRect y(0, 0, 299 - B_V_SCROLL_BAR_WIDTH , 149);

		BTextView* textView = new BTextView(x, "about_text", y, B_FOLLOW_NONE);
		BScrollView* scrollView = new BScrollView("scrollView", textView, B_FOLLOW_NONE, 0, false, true, B_FANCY_BORDER);
		but->AddChild(scrollView);
		textView->SetStylable (true);
 		textView->MakeEditable (false);

		BFont font(be_plain_font);
		font.SetSize(14.0);
		textView->SetFontAndColor(&font, B_FONT_ALL, &Blue);
		textView->Insert("XRS version ");
		textView->Insert(VERSION_NUMBER);
		textView->Insert(" (");
		textView->Insert(GIT_COMMIT);
		textView->Insert(")\nby Andrea Anzani\n");
		textView->SetFontAndColor(be_plain_font, B_FONT_ALL, &Black);
		textView->Insert("https://github.com/Freaxed/xrs-haiku\n\n");
		textView->Insert("FluidLite by Robin Lobel (LGPL-2.1-or-later)\n\n");
		textView->Insert("libsndfile library by Erik de Castro Lopo (LGPL-2.1-or-later)\n\n");
		textView->Insert("libsamplerate library by Erik de Castro Lopo (BSD-2-Clause License)\n\n");
		textView->Insert("VST Plug-In Technology by Steinberg\n\n");
	}
	else
	{
		but->AddChild( fStatusText = new BStringView(BRect(0, 0, r.Width(), 20),"", T_DIALOG_LOADING)); 
	}

	AddChild(but);
	w->SetViewBitmap(fAboutBitmap);
	MoveTo(centered_point);
	
}

AboutBox::~AboutBox()
{
	if (fAboutBitmap)
		delete fAboutBitmap;
}

void
AboutBox::setText(const char*t)
{
	if (fStatusText == NULL)
		return;

	if(Lock())
	{
		fStatusText->SetText(t); 
		Unlock();
 	}
}

