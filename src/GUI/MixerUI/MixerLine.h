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
#include <List.h>
#include "PlugWindow.h"
#include "ValuableManager.h"


#define MAX_VST 5

class PBus;
class VSTPlugin;
class StringBox;

class MixerLine : public BBox
{
public:
		MixerLine(PBus* bus, ValuableID volume, ValuableID pan, ValuableID meter);
		
		void	MessageReceived(BMessage* msg);
		void 	AttachedToWindow();
		void	ResetUI();

private:
		void		CreateVstItem(VSTPlugin* templ, BMessage* msg);
		void		CreateVstWindow(VSTItem*, uint8 position);
		void		UpdateItem(VSTItem*, uint8 position);

		BPopUpMenu*	fPopUp;
		BMenu*		fVSTMenu;
		PBus*		fBus;
		PlugWindow* fPlugWindows[MAX_VST];
		StringBox*	fBoxsList[MAX_VST];
};


#endif // MixerLine_H
