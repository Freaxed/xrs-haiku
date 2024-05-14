/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XrsWindow.h"
#include "Configurator.h"	

#define CONFIG_POSTFIX "_Config"

thread_id 	
XrsWindow::Run ()
{
	LoadConfig();
	return BWindow::Run();
}

void
XrsWindow::SaveConfig()
{		
	BMessage	xrsWindowConfig;
	xrsWindowConfig.AddRect("Frame", Frame());	

	SaveSettings(xrsWindowConfig);
	BString key = fName;
	key << CONFIG_POSTFIX;

	Config()->RemoveName(key);
	Config()->AddMessage(key, &xrsWindowConfig);
}

void 
XrsWindow::SaveSettings(BMessage&)
{
}

void
XrsWindow::LoadConfig()
{
	BString key = fName;
	key << CONFIG_POSTFIX;
	BMessage	xrsWindowConfig;
	if (Config()->FindMessage(key, &xrsWindowConfig) == B_OK)
	{
		BRect frame;
		if (xrsWindowConfig.FindRect("Frame", &frame) == B_OK)
		{
			MoveTo(frame.left, frame.top);
			ResizeTo(frame.Width(), frame.Height());
		}
		LoadSettings(xrsWindowConfig);
	}
	
}

void 
XrsWindow::LoadSettings(BMessage&)
{

}
		

		
void
XrsWindow::SetName(const char* name){ 
	fName = name; 
}
