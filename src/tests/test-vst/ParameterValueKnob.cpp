/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ParameterValueKnob.h"
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <stdio.h>

#define		FACTOR_I	((int32)1000)
#define		FACTOR_F	((float)1000.0f)
#define 	MSG_VALUE	'knob'

ParameterValueKnob::ParameterValueKnob()
					: BBox("ParameterValueKnob")
{
	
	mPot 	   = new APot("pot", new BMessage(MSG_VALUE), NULL, 0, FACTOR_I);		
	mParameter = new BStringView("parameter", "Parameter:");
	mValue     = new BStringView("value", "Value:");
	
	mPot->SetShowValue(false);
	
	BGroupLayout* vgr = BLayoutBuilder::Group<>(B_VERTICAL);

	vgr->AddView(mParameter)
	   ->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_BOTTOM));
	vgr->AddView(mPot);
	BLayoutItem* valu = vgr->AddView(mValue);
	valu->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP));	
	AddChild(vgr->View());
	
	SetValue(0.0f);
}

void		
ParameterValueKnob::SetParameterString(const char* name)
{
	mParameter->SetText(name);
}

void		
ParameterValueKnob::SetValueString(const char* name)
{
	mValue->SetText(name);
}

void
ParameterValueKnob::AttachedToWindow()
{
	mPot->SetTarget(this);
	BBox::AttachedToWindow();
}

void		
ParameterValueKnob::SetValue(float newValue)
{
	mPot->SetValue((int32)(newValue * FACTOR_F));
}

void
ParameterValueKnob::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case MSG_VALUE:
		{
			float value = (float)mPot->Value() / FACTOR_F;
			SetValue(value);
		}
		break;
		default:
			BBox::MessageReceived(msg);
		break;
	}
}
