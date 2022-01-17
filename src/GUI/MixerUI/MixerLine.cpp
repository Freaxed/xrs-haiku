/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MixerLine.h"
#include <StringView.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include "StereoVolumesView.h"
#include "ValuablePeakView.h"
#include "StringBox.h"
#include "Xed_Utils.h"

#define T_VIW_NAME			"VSTi Wrap"
#define T_VIW_CANT_FIND		"Can't find VSTi :"
#define T_VIW_NOVST			"<none>"

#define	SET_VSTI	'stvs'
#define	REMOVE_VSTI	'revs'
#define	SHOW_VSTI_WINDOW	'sviw'


MixerLine::MixerLine(const char* name, ValuableID volume, ValuableID pan, ValuableID meter) : BBox(name)
{
	BGroupLayout* hGroup = BLayoutBuilder::Group<>(B_HORIZONTAL);
	
	BGroupLayout* group = BLayoutBuilder::Group<>(B_VERTICAL);
	group->SetSpacing(10.0f);
	group->AddView(new BStringView("_line_name_", name))
		 ->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_VERTICAL_CENTER));
		 
	BGroupLayout* g1 = BLayoutBuilder::Group<>(B_HORIZONTAL);
	g1->SetSpacing(0.0f);
	StereoVolumesView* volView = new StereoVolumesView(volume, pan);
	g1->AddView(volView);
	
	group->AddItem(g1);
	
	fVSTMenu = new BMenu("Effects");
	fVSTMenu->AddItem(new BMenuItem("mdaJX10", new BMessage(SET_VSTI)));

	fPopUp = new BPopUpMenu("");
	fPopUp->AddItem(fVSTMenu);
	fPopUp->AddSeparatorItem();
	fPopUp->AddItem(new BMenuItem(T_VIW_NOVST, new BMessage(REMOVE_VSTI)));
	
	
	//VST
	BMessage extraInfo;
	extraInfo.AddInt32("vst:position", -1);
	
	for(uint8 i=0;i<MAX_VST;i++)
	{
		extraInfo.ReplaceInt32("vst:position", i);
		group->AddView(new StringBox("", fPopUp, &extraInfo));
	}
	
	ValuableVPeakView* peak = new ValuableVPeakView(meter, "_peak_");
	hGroup->AddView(peak);
//	  ->SetExplicitMaxSize(BSize(200, 24));
	  
	
	hGroup->AddItem(group);
	AddChild(hGroup->View());


}

void
MixerLine::AttachedToWindow()
{	
	fPopUp->SetTargetForItems(this);
	fVSTMenu->SetTargetForItems(this);
}

void	
MixerLine::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case REMOVE_VSTI:
		case SET_VSTI:
			msg->PrintToStream();
			{
				BString truncatedLabel("very long string here to be placed");
				StringBox*	box = NULL;
				if (msg->FindPointer("box", ((void**)&box)) == B_OK) {
					box->UpdateLabel(truncatedLabel.String());
				}
			}
		break;
		default:
		BBox::MessageReceived(msg);
		break;
	};
}

