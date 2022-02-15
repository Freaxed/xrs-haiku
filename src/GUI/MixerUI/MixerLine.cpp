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
#include "PBus.h"
#include "VSTHost.h"
#include "VSTItem.h"

#define T_VIW_NOVST			"<none>"

#define	SET_VSTI	'stvs'
#define	REMOVE_VSTI	'revs'
#define	SHOW_VSTI_WINDOW	'sviw'


MixerLine::MixerLine(PBus* bus, ValuableID volume, ValuableID pan, ValuableID meter) : BBox(bus->Name()), fBus(bus)
{
	BGroupLayout* hGroup = BLayoutBuilder::Group<>(B_HORIZONTAL);
	
	BGroupLayout* group = BLayoutBuilder::Group<>(B_VERTICAL);
	group->SetSpacing(10.0f);
	group->AddView(new BStringView("_line_name_", bus->Name()))
		 ->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_VERTICAL_CENTER));
		 
	BGroupLayout* g1 = BLayoutBuilder::Group<>(B_HORIZONTAL);
	g1->SetSpacing(0.0f);
	StereoVolumesView* volView = new StereoVolumesView(volume, pan);
	g1->AddView(volView);
	
	group->AddItem(g1);
	
	fVSTMenu = new BMenu("Effects");
	const BList*	vstList = fBus->Effector()->GetEffectsList();
	for (uint8 i=0;i<vstList->CountItems();i++) {
		BMessage* info = new BMessage(SET_VSTI);
		info->AddInt16("vst:id", (int16)i);
		VSTPlugin* plug = (VSTPlugin*)vstList->ItemAt(i);
		fVSTMenu->AddItem(new BMenuItem(plug->EffectName(), info));
	}
	
	

	fPopUp = new BPopUpMenu("");
	fPopUp->AddItem(fVSTMenu);
	fPopUp->AddSeparatorItem();
	fPopUp->AddItem(new BMenuItem(T_VIW_NOVST, new BMessage(REMOVE_VSTI)));
	
	
	//VST
	BMessage extraInfo;
	extraInfo.AddInt8("vst:position", -1);
	
	for(uint8 i=0;i<MAX_VST;i++)
	{
		extraInfo.ReplaceInt8("vst:position", i);
		fBoxsList[i] = new StringBox("", fPopUp, &extraInfo);
		group->AddView(fBoxsList[i]);
	}
	
	ValuableVPeakView* peak = new ValuableVPeakView(meter, "_peak_");
	hGroup->AddView(peak);
//	  ->SetExplicitMaxSize(BSize(200, 24));
	  
	
	hGroup->AddItem(group);
	AddChild(hGroup->View());
	
	for (uint i=0;i<MAX_VST;i++) {
		fPlugWindows[i] = NULL; 
	}


}

void
MixerLine::AttachedToWindow()
{	
	fPopUp->SetTargetForItems(this);
	fVSTMenu->SetTargetForItems(this);
}

void	
MixerLine::ResetUI()
{
	// let's synch bus effects with the line
	PEffector* effector = fBus->Effector();
	for (uint i=0;i<MAX_VST;i++) {
		VSTItem* item = effector->GetVstAtPosition(i);
		UpdateItem(item, i);
	}
	
}

void		
MixerLine::UpdateItem(VSTItem* item, uint8 position)
{
	StringBox*	box = fBoxsList[position];
	box->UpdateLabel(item == NULL ? NULL : item->EffectName());
	CreateVstWindow(item, position);
}

void		
MixerLine::CreateVstWindow(VSTItem* item, uint8 position)
{
	assert(position < MAX_VST);
	PlugWindow* prevWindow = fPlugWindows[position];
	if (prevWindow && prevWindow->Lock())
		prevWindow->Quit();
	
	fPlugWindows[position] = NULL;
	
	if (item) {
		fPlugWindows[position] = new PlugWindow(item, true);
		fPlugWindows[position]->Show();
	}
}


void		
MixerLine::CreateVstItem(VSTPlugin* templ, BMessage* msg)
{
	BMessage extra;
	int8    position = -1;
	if (msg->FindMessage("extra", &extra) == B_OK && 
	    extra.FindInt8("vst:position", &position) == B_OK)
	{
		VSTItem* item = fBus->Effector()->CreateVstAtPosition(templ, position);
		UpdateItem(item, position);
	}	
	else
	{
		LogError("Can't create VST from MixerLine: no extra info or vst:position!");
		if (Logger::IsErrorEnabled()) {
			msg->PrintToStream();
		}
	}
}

void	
MixerLine::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case REMOVE_VSTI:
		{
			CreateVstItem(NULL, msg);
		}
		break;
		case SET_VSTI:
			{
						
				const BList*	vstList = fBus->Effector()->GetEffectsList();
				int16 vstID = msg->FindInt16("vst:id");
				VSTPlugin* plug = (VSTPlugin*)vstList->ItemAt(vstID);
				
				CreateVstItem(plug, msg);
			}
		break;
		default:
		BBox::MessageReceived(msg);
		break;
	};
}

