/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _VSTConfigureView_H_
#define _VSTConfigureView_H_

#include "VSTHost.h"

#include	<Box.h>

class BMenuField;
class VSTItem;
class BMenu;
class BMessageRunner;

class VSTConfigureView : public BBox {
public:
					VSTConfigureView (VSTPlugin * plugin);
	//virtual	void	MessageReceived (BMessage *msg);

			BMenu*		GetMenu();
			void		ResetUI();
			bool		HasNativeUI();
	
	BMenuField*		fMidiSelect;
	VSTPlugin*		fPlugin;

private:
		bool nativeUI;
		
};

#endif
