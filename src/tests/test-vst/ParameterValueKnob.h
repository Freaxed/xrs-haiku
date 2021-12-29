/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ParameterValueKnob_H
#define ParameterValueKnob_H


#include <Box.h>
#include <StringView.h>
#include "APot.h"


class ParameterValueKnob : public BBox {

public:
						ParameterValueKnob();
virtual		void		MessageReceived(BMessage *msg);
virtual		void		AttachedToWindow();
virtual		void		SetValue(float newValue);
			
			void		SetParameterString(const char* name);
			void		SetValueString(const char* name);
			
private:
		BStringView*	mParameter;
		BStringView*	mValue;
		APot*			mPot;

};


#endif // ParameterValueKnob_H
