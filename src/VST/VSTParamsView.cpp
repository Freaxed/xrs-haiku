#include "VSTParamsView.h"
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>
#include "VSTKnob.h"
#include <StringView.h>
#include "vestige.h"

#define MAX_PER_ROW 10

/*
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
*/

VSTParamsView::VSTParamsView(VSTPlugin* plugin)
              : BView("VSTParamsView", B_WILL_DRAW|B_FRAME_EVENTS, NULL), fPlugin(plugin)
{
	BGroupLayout* group = BLayoutBuilder::Group<>(B_VERTICAL);
	SetLayout(group);
	grid = BLayoutBuilder::Grid<>();

	group->AddView(new BStringView("_fx_name_", plugin->EffectName()))
		 ->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_VERTICAL_CENTER));
	group->AddItem(grid);
	grid->SetSpacing(1, 1);
	
	//fEffect->dispatcher (fEffect, effOpen, 0, 0, 0, 0.);
	short *rect = 0;
	VSTEffect*	fEffect = fPlugin->Effect();
	fEffect->dispatcher (fEffect, effEditGetRect, 0, 0, &rect, 0);
	if (rect)
	{
		BView * host = new BView (BRect(0,0, 500, 500), "editor", 0, 0);
		grid->AddView (host, 0, 0);
		fEffect->dispatcher (fEffect, effEditOpen, 0, 0, host, 0);
	} else
	{
	int count = plugin->ParametersCount();
	for (int p = 0; p < count; p++) {
		VSTKnob* knob = new VSTKnob(plugin->Parameter(p));
		grid->AddView(knob, p % MAX_PER_ROW, p / MAX_PER_ROW);
	}
	}

	ResizeTo(group->BasePreferredSize());
}

void VSTParamsView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
}

BMenu*	
VSTParamsView::CreateMenu()	
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


void VSTParamsView::MessageReceived (BMessage *msg)
{
//	if (msg->what == 'Idle')
//		fEffect->dispatcher (fEffect, effEditIdle, 0, 0, 0, 0.f);

	if (msg->what == 'Prog')
	{
		msg->PrintToStream();
		int32	index=msg->FindInt32("index");
		fPlugin->Effect()->dispatcher (fPlugin->Effect(), VST_SET_PROGRAM, 0, index, 0, 0.f);
		ResetUIFromPlugin();
		//fPlugin->StopAll();
		
	}
	else if (msg->what == 'Midi')
	{
//		int32	id;
//		if (msg->FindInt32 ("id", &id) == B_OK)
//			fPlugin->SetMidiProducer (id);
	}
	
	else
		BView::MessageReceived (msg);
}


void
VSTParamsView::ResetUIFromPlugin()
{
	int index = 0;
	BView * child;
	while ((child = grid->View()->ChildAt (index++)) != 0)
	{
		//printf("Debug: %s\n", child->Name());
		VSTKnob * knob = dynamic_cast<VSTKnob*> (child);
		if (knob){
			//printf("Debug: %s\n", knob->Name());
			knob->LoadParameter ();
			// special case!
			//if(fPlugin->name.Compare(BString("mdaJX10"),7)==0 & index==4) slider->SetValue(0);
				
		}
	}
}
