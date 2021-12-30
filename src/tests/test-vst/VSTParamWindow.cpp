/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "VSTParamWindow.h"
#include <GroupLayout.h>
#include <StringView.h>
#include <LayoutItem.h>
#include <GridLayout.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <ScrollView.h>
#include <Box.h>
#include "APot.h"
#include "VSTKnob.h"
#include "VSTParamsView.h"


VSTParamWindow::VSTParamWindow(VSTPlugin* plugin)
	:	BWindow(BRect(100,100,400,400),"VSTParamWindow",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	BGridLayout* grid = BLayoutBuilder::Grid<>();
	SetLayout(grid);
	/*int count = plugin->ParametersCount();

	for (int p = 0; p < count; p++) {
		grid->AddView(new VSTKnob(plugin->Parameter(p)), p%5, p/5);		
	}*/
	VSTParamsView* view = new VSTParamsView(plugin);
	grid->AddView(view);
	
	float w,h;
	view->GetPreferredSize(&w,&h);
	printf("size %f %f\n", w, h);
	ResizeTo(w, h);
}
