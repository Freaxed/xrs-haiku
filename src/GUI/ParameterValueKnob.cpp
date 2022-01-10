/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ParameterValueKnob.h"
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <stdio.h>
#include "PotViewer.h"

#define		FACTOR_I	((int32)1000)
#define		FACTOR_F	((float)1000.0f)
#define 	MSG_VALUE	'knob'

ParameterValueKnob::ParameterValueKnob(const char* name)
					: BBox(name), AsyncDisplayValue(this)
{
	
	mPot 	   = new APot("pot", new BMessage(MSG_VALUE), NULL, 0, FACTOR_I);		
	mLabel     = new BStringView("label", "Label");
	
	mPot->SetDisplayValue(this);
	
	vgr = BLayoutBuilder::Group<>(B_VERTICAL);

	vgr->AddView(mPot);
    vgr->AddView(mLabel)
	   ->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP));	
	
	AddChild(vgr->View());
	SetValue(0.0f);
	SetSpacing(0);

}

void				
ParameterValueKnob::SetSpacing(float spacing) 
{
	vgr->SetSpacing(spacing);
}
void	
ParameterValueKnob::SetKnobSize(float size) 
{ 
	mPot->SetSize(size);
}

APot*		
ParameterValueKnob::GetKnob()
{
	return mPot;
}

void		
ParameterValueKnob::SetFontSize(float size)
{
	BBox::SetFontSize(size);
	mLabel->SetFontSize(size);
}

void		
ParameterValueKnob::SetLabelText(const char* name)
{
	mLabel->SetText(name);
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
ParameterValueKnob::ValueDisplay(float value, BString& display)
{
	display << (int32)(value * 100.0f) << "%";
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
		case MSG_ASYNC_DV_SHOW:
		{
			PotViewer::Get()->InitShow(mLabel);
		}
		break;
		case MSG_ASYNC_DV_VALUE:
		{
			int32 value;
			if (msg->FindInt32("be:value", &value) == B_OK) {
				BString display;
				ValueDisplay((float)value / FACTOR_F, display);
				PotViewer::Get()->SetValue(display);
			}
		}
		break;
		case MSG_ASYNC_DV_HIDE:
		{
			PotViewer::Get()->InitHide();
		}
		break;
		default:
			BBox::MessageReceived(msg);
		break;
	}
}
