/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef StereoVolumesView_H
#define StereoVolumesView_H


#include <SupportDefs.h>
#include <View.h>
#include <ValuableManager.h>

class	XPot;

class StereoVolumesView : public BView {
public:
		StereoVolumesView(ValuableID gainId, ValuableID panId);
		
		virtual void AttachedToWindow();
		

protected:
		XPot*			GetGainKnob() { return fGainKnob; }
		XPot*			GetPanKnob()  { return fPanKnob;  }
		
		
private:
		XPot*			fGainKnob;
		XPot*			fPanKnob;
};


#endif // StereoVolumesView_H
