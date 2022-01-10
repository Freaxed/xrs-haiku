/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "StereoVolumesView.h"
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <StringView.h>
#include "XPot.h"



StereoVolumesView::StereoVolumesView(ValuableID gainId, ValuableID panId): BView("", B_WILL_DRAW)
{
	BGroupLayout* group = BLayoutBuilder::Group<>(B_HORIZONTAL);
	SetLayout(group);
	fGainKnob = new XPot("_gain_", gainId,    0, 100);
	fPanKnob  = new XPot("_pan_",   panId, -100, 100);
	fPanKnob->SetSize(24.0f);
	GetLayout()->AddView(fGainKnob);
	GetLayout()->AddView(fPanKnob);

	ResizeTo(group->BasePreferredSize());		
}
