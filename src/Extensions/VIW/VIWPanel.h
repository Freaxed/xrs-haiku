/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"PlugPanel.h"
#include	<Menu.h>
#include	<MenuItem.h>
#include	<MenuField.h>
#include	<Box.h>
#include	<Message.h>
#include	<List.h>
#include	<StringView.h>

class	XDigit;
class	VIWTrack;
class 	BPictureButton;

class VIWPanel: public PlugPanel
{
	public:
				VIWPanel(const BList*);
	void 	Reset(Track* tr);
	void	AttachedToWindow();
	void	MessageReceived(BMessage *msg);
	
	private:
		XDigit*	ch;
		VIWTrack*	myTrack;
		BMenu*	menu;
		BMenuField*	field;
		const BList	*vst_list;
		BPictureButton*	brez;
		BRect	but;
		BBox *sampler;
};
