/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "VSTConfigureView.h"
#include "VSTItem.h"
#include "ParameterSlider.h"

#include <Window.h>
#include <Messenger.h>
#include <MessageRunner.h>
#include <StringView.h>
#include <TextControl.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <MenuBar.h>
#include <MidiRoster.h>
#include <MidiProducer.h>
#include <stdio.h>

#define	X_WIN_ZOOM		'xwzo'

const float kConfigHeight = 5;

VSTConfigureView::VSTConfigureView (VSTItem * plugin) :
	BView (BRect (0, 0, 10, 10), B_EMPTY_STRING, 0, B_WILL_DRAW),
	fMidiSelect (0), fPlugin (plugin), fMessageRunner (0), fEffect (plugin->fEffect)
{


	
	fEffect->dispatcher (fEffect, effOpen, 0, 0, 0, 0.);
	short *rect = 0;
	fEffect->dispatcher (fEffect, effEditGetRect, 0, 0, &rect, 0);
	if (rect)
	{
		nativeUI=true;
		ResizeTo (float (rect[3] - 1), float (rect[2] - 1) );
		BRect frame (Bounds ());
		//frame.top += kConfigHeight;
		BView * host = new BView (frame, 0, 0, 0);
		AddChild (host);
		fEffect->dispatcher (fEffect, effEditOpen, 0, 0, host, 0);
	}
	else
	{
		nativeUI=false;
		int count = fEffect->numParams;
		const float kParameterWidth = 275;
		const float kParameterHeight = 40;
		
		ResizeTo (kParameterWidth, kParameterHeight * count + kConfigHeight);
		
		BRect frame (0, kConfigHeight, kParameterWidth, kConfigHeight + 39);
		
		for (int p = 0; p < count; p++)
		{
			ParameterSlider *  ps = new ParameterSlider (frame, fEffect, p);
			AddChild (ps);
			frame.top += kParameterHeight;
			frame.bottom += kParameterHeight;
		}
	}
}
bool
VSTConfigureView::HasNativeUI()
{
	return nativeUI;
}
void VSTConfigureView::AttachedToWindow ()
{
	BWindow * window = Window ();
	
	if (window)
		{
			window->SetFlags (window->Flags () | B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_NOT_RESIZABLE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS);
			//window->SetType(B_FLOATING_WINDOW);
		}
	
	BMessenger messenger (this);
	fMessageRunner = new BMessageRunner (messenger, new BMessage ('Idle'), 10000, -1);
	SetViewColor (Parent ()->ViewColor ());
}

BMenu*	
VSTConfigureView::GetMenu()	
{	
	char	program[32];
	
	fEffect->dispatcher (fEffect, effGetProgramName, 0, 0, program, 0);
	
	
	
	BMenu* popup = new BMenu("Standard");
	popup->SetFont(be_plain_font);

	
	int32 currentProgram = fEffect->dispatcher (fEffect, effGetProgram, 0, 0, 0, 0.f);
	int		progCount = fEffect->numPrograms;
	char	name[32];
	for (int prog = 0; prog < progCount; prog++)
	{
		fEffect->dispatcher (fEffect, effSetProgram, 0, prog, 0, 0.f);
		fEffect->dispatcher (fEffect, effGetProgramName, 0, 0, name, 0);
		popup->AddItem (new BMenuItem (name, new BMessage ('Prog')));
	}
	fEffect->dispatcher (fEffect, effSetProgram, 0, currentProgram, 0, 0.f);
	
	popup->SetTargetForItems(this);
	return popup;
	
}

void VSTConfigureView::DetachedFromWindow ()
{
	fEffect->dispatcher (fEffect, effEditClose, 0, 0, 0, 0.f);
	if (fMessageRunner)
		delete fMessageRunner;
}

void VSTConfigureView::MessageReceived (BMessage *msg)
{
	if (msg->what == 'Idle')
		fEffect->dispatcher (fEffect, effEditIdle, 0, 0, 0, 0.f);

	else if (msg->what == 'Prog')
	{
		int32	index=msg->FindInt32("index");
		fEffect->dispatcher (fEffect, effSetProgram, 0, index, 0, 0.f);
		ResetUI();
		//fPlugin->StopAll();
		
	}
	else if (msg->what == 'Midi')
	{
		/*int32	id;
		if (msg->FindInt32 ("id", &id) == B_OK)
			fPlugin->SetMidiProducer (id);*/
	}
	
	else
		BView::MessageReceived (msg);
}

void 
VSTConfigureView::WindowActivated (bool state)
{
	if (state)
		fEffect->dispatcher (fEffect, effEditTop, 0, 0, 0, 0.f);
	else
		fEffect->dispatcher (fEffect, effEditSleep, 0, 0, 0, 0.f);
}
void
VSTConfigureView::ResetUI()
{
			int index = 0;
			BView * child;
			while ((child = ChildAt (index++)) != 0)
			{
				ParameterSlider * slider = dynamic_cast<ParameterSlider*> (child);
				if (slider){
					slider->LoadParameter ();
					// special case!
					if(fPlugin->name.Compare(BString("mdaJX10"),7)==0 & index==4) slider->SetValue(0);
						
					}
			}
}

