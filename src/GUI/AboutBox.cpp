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
#include <Alert.h>
#include <Box.h>
#include <Window.h>
#include <Font.h>
#include <TextView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include "Colors.h"

#define PREFS_H 300
#define PREFS_L 300


AboutBox::AboutBox(bool big_mode):BWindow(BRect(100, 100, 100 + PREFS_L, 100 + PREFS_H), 
								   "About XRS", 
								   B_TITLED_WINDOW_LOOK, 
								   B_MODAL_APP_WINDOW_FEEL, 
								   B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fStatusText = NULL;
	
	if(!big_mode) 
		SetLook(B_NO_BORDER_WINDOW_LOOK);
	
	// Initialize bitmap resources
	BView *sporca=new BView(BRect(0,0,10,10),"",B_FOLLOW_NONE,B_WILL_DRAW);
	AddChild(sporca);
	XUtils::ResetList(sporca);
	RemoveChild(sporca);

	// Create logo view
	BView* logoView = new BView("LogoView", B_WILL_DRAW);
	logoView->SetExplicitMinSize(BSize(PREFS_L, PREFS_H));
	logoView->SetExplicitMaxSize(BSize(PREFS_L, PREFS_H));
	
	// Create bitmap after we know the size
	fAboutBitmap = XUtils::CreateBootBitmap(BRect(0, 0, PREFS_L, PREFS_H));
	
	BBox* but = new BBox("UnderTheLogo");
	
	if(big_mode)
	{
		BTextView* textView = new BTextView("about_text");
		textView->SetStylable(true);
 		textView->MakeEditable(false);
		textView->SetExplicitMinSize(BSize(PREFS_L - 20, 149));
		textView->SetExplicitMaxSize(BSize(PREFS_L - 20, 149));

		BFont font(be_plain_font);
		font.SetSize(14.0);
		textView->SetFontAndColor(&font, B_FONT_ALL, &Blue);
		textView->Insert("XRS version ");
		textView->Insert(VERSION_NUMBER);
		textView->Insert(" (");
		textView->Insert(CODENAME);
		textView->Insert(")\nby Andrea Anzani\n");
		textView->SetFontAndColor(be_plain_font, B_FONT_ALL, &Black);
		textView->Insert("https://github.com/Freaxed/xrs-haiku\n\n");
		textView->Insert("FluidLite by Robin Lobel (LGPL-2.1-or-later)\n\n");
		textView->Insert("libsndfile library by Erik de Castro Lopo (LGPL-2.1-or-later)\n\n");
		textView->Insert("libsamplerate library by Erik de Castro Lopo (BSD-2-Clause License)\n\n");
		textView->Insert("VST Plug-In Technology by Steinberg\n\n");
		
		BScrollView* scrollView = new BScrollView("scrollView", textView, 0, false, true, B_FANCY_BORDER);
		scrollView->SetExplicitMinSize(BSize(PREFS_L - 10, 169));
		scrollView->SetExplicitMaxSize(BSize(PREFS_L - 10, 169));
		
		BLayoutBuilder::Group<>(but, B_VERTICAL)
			.SetInsets(B_USE_DEFAULT_SPACING)
			.Add(scrollView);
	}
	else
	{
		fStatusText = new BStringView("", T_DIALOG_LOADING);
		BLayoutBuilder::Group<>(but, B_VERTICAL)
			.SetInsets(B_USE_DEFAULT_SPACING)
			.Add(fStatusText);
	}

	// Build main window layout
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(logoView)
		.Add(but);
	
	// Set bitmap AFTER view is added to window
	logoView->SetViewBitmap(fAboutBitmap);
		
	// Center window
	BPoint centered_point = BAlert::AlertPosition(PREFS_L, big_mode ? PREFS_H + 150 : PREFS_H + 20);
	MoveTo(centered_point);
	ResizeTo(PREFS_L, big_mode ? PREFS_H + 150 : PREFS_H + 20);
	
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

