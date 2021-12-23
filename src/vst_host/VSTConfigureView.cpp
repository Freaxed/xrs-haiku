/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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

const float kConfigHeight = 5;

VSTConfigureView::VSTConfigureView (VSTPlugin * plugin) :
	BBox (BRect (0, 0, 10, 10), B_EMPTY_STRING, 0, B_WILL_DRAW),
	fMidiSelect (0), fPlugin (plugin)
{
	{
		nativeUI = false;
		int count = plugin->ParametersCount();
		const float kParameterWidth = 275;
		const float kParameterHeight = 50;
		
		ResizeTo (kParameterWidth, kParameterHeight * count + kConfigHeight);
		
		BRect frame (2, kConfigHeight, kParameterWidth - 2 , kConfigHeight + kParameterHeight);
		
		for (int p = 0; p < count; p++)
		{
			ParameterSlider *  ps = new ParameterSlider (frame, plugin->Parameter(p));
			AddChild (ps);
			frame.top    += kParameterHeight;
			frame.bottom += kParameterHeight;
		}
	}
}
bool
VSTConfigureView::HasNativeUI()
{
	return nativeUI;
}


BMenu*	
VSTConfigureView::GetMenu()	
{	/*
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
	return popup;*/
	return NULL;
	
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

