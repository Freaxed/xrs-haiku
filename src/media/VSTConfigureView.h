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

#ifndef __audioeffectx__
#include "audioeffectx.h"
#endif

#include	<View.h>

class BMenuField;
class VSTInstrumentPlugin;
class VSTItem;
class BMenu;
class BMessageRunner;

class VSTConfigureView : public BView {
public:
					VSTConfigureView (VSTItem * plugin);
	virtual	void 	AttachedToWindow ();
	virtual	void 	DetachedFromWindow ();
	virtual	void	MessageReceived (BMessage *msg);
	virtual	void	WindowActivated (bool state);
			BMenu*	GetMenu();
			void		ResetUI();
			bool		HasNativeUI();
	
	BMenuField*		fMidiSelect;
	VSTItem*		fPlugin;
	BMessageRunner*	fMessageRunner;
	AEffect*		fEffect;

private:
		bool nativeUI;
		
};

#endif
