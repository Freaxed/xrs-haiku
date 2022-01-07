/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ParameterValueKnob_H
#define ParameterValueKnob_H


#include <Box.h>
#include <StringView.h>
#include "APot.h"
class BGroupLayout;
class KnobDisplayValue;

class ParameterValueKnob : public BBox, public AsyncDisplayValue {

public:
						ParameterValueKnob(const char* name = "ParameterValueKnob");
virtual		void		MessageReceived(BMessage *msg);
virtual		void		AttachedToWindow();
virtual		void		SetValue(float newValue);
virtual		void		ValueDisplay(float value, BString& display);
			
			void		SetLabelText(const char* name);
			void		SetFontSize(float size);
			void		SetSpacing(float spacing);
			void		SetKnobSize(float size);
			APot*		GetKnob();
			
private:
		BStringView*	mLabel;
		APot*			mPot;
		BGroupLayout* 	vgr;
		KnobDisplayValue*	mKnobDisplayValue;
};


#endif // ParameterValueKnob_H
