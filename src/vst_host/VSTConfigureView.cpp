/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "VSTConfigureView.h"
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
//#include "VSTEnums.h"

#define	X_WIN_ZOOM		'xwzo'
const float kParameterWidth  = 60;
const float kParameterHeight = 300;

#define VST_

VSTConfigureView::VSTConfigureView (VSTPlugin * plugin) :
	BBox (BRect (0, 0, 10, 10), B_EMPTY_STRING, 0, B_WILL_DRAW),
	fMidiSelect (0), fPlugin (plugin)
{
	nativeUI = false; //for now.
	
	int count = plugin->ParametersCount();

	
	ResizeTo (kParameterWidth * count, kParameterHeight);
	
	BRect frame (2, 2, 2 + kParameterWidth, kParameterHeight - 2);
	
	for (int p = 0; p < count; p++)
	{
		ParameterSlider *  ps = new ParameterSlider (frame, plugin->Parameter(p));
		AddChild (ps);
//		frame.top    += kParameterHeight;
//		frame.bottom += kParameterHeight;
		frame.OffsetBy(kParameterWidth, 0);
	}
}
bool
VSTConfigureView::HasNativeUI()
{
	return nativeUI;
}


BMenu*	
VSTConfigureView::GetMenu()	
{
	VSTEffect*	fEffect = fPlugin->Effect();
	
	char	program[32];
	
	fEffect->dispatcher (fEffect, VST_GET_PROGRAM_NAME, 0, 0, program, 0);
	
	BMenu* popup = new BMenu("Standard");
	popup->SetFont(be_plain_font);

	
	int32 currentProgram = fEffect->dispatcher (fEffect, VST_GET_PROGRAM, 0, 0, 0, 0.f);
	int		progCount = fEffect->numPrograms;
	char	name[32];
	for (int prog = 0; prog < progCount; prog++)
	{
		fEffect->dispatcher (fEffect, VST_SET_PROGRAM,      0, prog, 0,    0.f);
		fEffect->dispatcher (fEffect, VST_GET_PROGRAM_NAME, 0, 0,    name, 0);
		popup->AddItem (new BMenuItem (name, new BMessage ('Prog')));
	}
	fEffect->dispatcher (fEffect, VST_SET_PROGRAM, 0, currentProgram, 0, 0.f);
	
	popup->SetTargetForItems(this);
	return popup;
}

/*
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
//		int32	id;
//		if (msg->FindInt32 ("id", &id) == B_OK)
//			fPlugin->SetMidiProducer (id);
	}
	
	else
		BBox::MessageReceived (msg);
}
*/

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
			//if(fPlugin->name.Compare(BString("mdaJX10"),7)==0 & index==4) slider->SetValue(0);
				
		}
	}
}

