#ifndef VSTPARAMSVIEW_H
#define VSTPARAMSVIEW_H

#include <View.h>
#include "VSTHost.h"

class VSTParamsView : public BView {
public:
		VSTParamsView(VSTPlugin* plugin);
		
		void AttachedToWindow();
};



#endif
