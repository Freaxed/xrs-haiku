/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "MidiTools.h"

#include <Bitmap.h>
#include <AppFileInfo.h>
#include <Application.h>
#include <Roster.h>

const char*	LARGE_ICON_NAME = "be:large_icon";
const char*	MINI_ICON_NAME = "be:mini_icon";
const uint32 LARGE_ICON_TYPE = 'ICON';
const uint32 MINI_ICON_TYPE = 'MICN';

void		
PrepareMessage(BMessage* msg)
{
	BBitmap* largeIcon = new BBitmap(BRect(0,0,31,31), B_CMAP8);
	BBitmap* miniIcon = new BBitmap(BRect(0,0,15,15), B_CMAP8);
	GetIcons(largeIcon, miniIcon);
	AddIcons(msg, largeIcon, miniIcon);
	
}
void 
AddIcons(BMessage* msg, BBitmap* largeIcon, BBitmap* miniIcon) 
{
	if (! msg->HasData(LARGE_ICON_NAME, LARGE_ICON_TYPE)) {
		msg->AddData(LARGE_ICON_NAME, LARGE_ICON_TYPE, largeIcon->Bits(),
			largeIcon->BitsLength());
	} else {
		msg->ReplaceData(LARGE_ICON_NAME, LARGE_ICON_TYPE, largeIcon->Bits(),
			largeIcon->BitsLength());
	}
	if (! msg->HasData(MINI_ICON_NAME, MINI_ICON_TYPE)) {
		msg->AddData(MINI_ICON_NAME, MINI_ICON_TYPE, miniIcon->Bits(),
			miniIcon->BitsLength());
	} else {
		msg->ReplaceData(MINI_ICON_NAME, MINI_ICON_TYPE, miniIcon->Bits(),
			miniIcon->BitsLength());
	}
}
void GetIcons(BBitmap* largeIcon, BBitmap* miniIcon)
{
	app_info ai;
	status_t err;
	
	// get the icons from the app file info of the
	// currently running app
	err = be_app->GetAppInfo(&ai);
	if (err == B_OK) {
		BFile file(&(ai.ref), B_READ_ONLY);
		err = file.InitCheck();
		if (err == B_OK) {
			BAppFileInfo afi(&file);
			err = afi.InitCheck();
			if (err == B_OK) {
				afi.GetIcon(largeIcon, B_LARGE_ICON);
				afi.GetIcon(miniIcon, B_MINI_ICON);
			}
		}
	}
}
