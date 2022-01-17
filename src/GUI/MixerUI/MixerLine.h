/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MixerLine_H
#define MixerLine_H


#include <SupportDefs.h>
#include <Box.h>
#include <Menu.h>
#include <PopUpMenu.h>
#include "ValuableManager.h"

#define MAX_VST 5

class MixerLine : public BBox
{
public:
		MixerLine(const char* name, ValuableID volume, ValuableID pan, ValuableID meter);
		
		void	MessageReceived(BMessage* msg);
		void 	AttachedToWindow();

private:
		BPopUpMenu*	fPopUp;
		BMenu*		fVSTMenu;
};


#endif // MixerLine_H
