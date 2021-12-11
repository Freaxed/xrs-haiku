/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ValuablePeakView.h"

ValuablePeakView::ValuablePeakView(ValuableID id, const char* name)
				 :PeakView(name, false, false), vID(id) {


}

void				
ValuablePeakView::MessageReceived(BMessage* message) {
		if (message->what == MSG_VALUABLE_CHANGED)
		{
			float value_l;
			float value_r;
			if (ValuableTools::SearchValues(vID, message, &value_l, &value_r)){
					SetMax(value_l, 0);
					SetMax(value_r, 1);			
			}
			fGotData = true;
		}
	PeakView::MessageReceived(message);
}

void
ValuablePeakView::AttachedToWindow(){
	PeakView::AttachedToWindow();
	ValuableManager::Get()->RegisterValuableReceiver(vID, this);
}

void
ValuablePeakView::DetachedFromWindow(){
	ValuableManager::Get()->UnregisterValuableReceiver(vID, this);
	PeakView::DetachedFromWindow();
}
