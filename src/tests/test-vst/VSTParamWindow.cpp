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
	VSTParamsView*	vstPar = new VSTParamsView(plugin);
	AddChild(vstPar);
	ResizeTo(vstPar->Frame().Width(), vstPar->Frame().Height());
}
