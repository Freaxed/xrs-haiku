/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ValuablePeakView_H
#define ValuablePeakView_H


#include <SupportDefs.h>
#include "PeakView.h"
#include "VertPeakView.h"
#include "ValuableManager.h"

template<class TYPE>
class ValuablePeakView : public TYPE {
public:
		ValuablePeakView(ValuableID id, const char* name)
			:TYPE(name, false, false), vID(id) {}
		
		void				MessageReceived(BMessage* message);
		void				AttachedToWindow();
		void				DetachedFromWindow();

private:
		ValuableID	vID;

};

template<class TYPE>
void ValuablePeakView<TYPE>::AttachedToWindow() {
	TYPE::AttachedToWindow();
	ValuableManager::Get()->RegisterValuableReceiver(vID, this);
}

template<class TYPE>
void ValuablePeakView<TYPE>::MessageReceived(BMessage* message) {
		if (message->what == MSG_VALUABLE_CHANGED)
		{
			float value_l;
			float value_r;
			if (ValuableTools::SearchValues(vID, message, &value_l, &value_r)){
					TYPE::SetMax(value_l, 0);
					TYPE::SetMax(value_r, 1);			
			}
			TYPE::fGotData = true;
		}
	TYPE::MessageReceived(message);

}

template<class TYPE>
void ValuablePeakView<TYPE>::DetachedFromWindow(){
	ValuableManager::Get()->UnregisterValuableReceiver(vID, this);
	TYPE::DetachedFromWindow();
}

typedef ValuablePeakView<PeakView> 		ValuableHPeakView;
typedef ValuablePeakView<VertPeakView>  ValuableVPeakView;



#endif // ValuablePeakView_H
