#ifndef VSTKNOB_H
#define VSTKNOB_H

#include "ParameterValueKnob.h"
class VSTParameter;

class VSTKnob : public ParameterValueKnob  {
	
	public:
				VSTKnob(VSTParameter*);
	virtual		void		SetValue(float newValue);
	virtual		void		ValueDisplay(float value, BString& display);
	private:
				VSTParameter*	fParameter;
};
	

#endif
