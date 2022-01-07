#ifndef VSTPARAMSVIEW_H
#define VSTPARAMSVIEW_H

#include <View.h>
#include "VSTHost.h"
#include <Menu.h>

class BGridLayout;

class VSTParamsView : public BView {
public:
		VSTParamsView(VSTPlugin* plugin);
		
		void AttachedToWindow();
		
		void MessageReceived (BMessage *msg);
		
		BMenu*	CreateMenu();
		void	ResetUIFromPlugin();
		
private:
		VSTPlugin*	fPlugin;
		BGridLayout*	grid;
};



#endif
