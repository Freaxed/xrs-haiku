/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ValuablePeakView_H
#define ValuablePeakView_H


#include <SupportDefs.h>
#include "PeakView.h"
#include "ValuableManager.h"

class ValuablePeakView : public PeakView, public ValuableReceiver {
public:
		ValuablePeakView(ValuableID id, const char* name);
		
		void				MessageReceived(BMessage* message);
		void				AttachedToWindow();
		void				DetachedFromWindow();
		
		BHandler*	GetHandler() { return this; }

private:
		ValuableID	vID;

};


#endif // ValuablePeakView_H
