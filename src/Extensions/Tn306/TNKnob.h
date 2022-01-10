/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TNKnob_H
#define TNKnob_H


#include <SupportDefs.h>
#include "ParameterValueKnob.h"


class TNKnob : public ParameterValueKnob  {
public:
							TNKnob(const char* label, TB303Knobs::update_value fun, TB303Knobs* knobs):
							fFunction(fun), fKnobs(knobs)
							{
								SetLabelText(label);
								SetFontSize(10.0f);
								SetKnobSize(32.0f);
								SetBorder(B_NO_BORDER);
							}
							
	virtual		void		SetValue(float newValue) {
								ParameterValueKnob::SetValue(newValue);
								(fKnobs->*fFunction)(newValue);
							}
	
private:
				TB303Knobs::update_value	fFunction;	
				TB303Knobs*		fKnobs;
};


#endif // TNKnob_H
