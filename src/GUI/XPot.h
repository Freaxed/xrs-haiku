/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _XPot_h
#define _XPot_h

#include 	"APot.h"
#include	"ValuableView.h"
#include	"ValuableController.h"

#define	SB_MSG	'sbmp'

class XPot : public APot, public ValuableView, public ValuableController
{
public:
	XPot(BRect frame, const char *name,
		ValuableID id, int32 valuable_channel, 
		BMessage *satte,
		int32 minValue, int32 maxValue,
		BBitmap *p1 = NULL ,BBitmap *p2 = NULL);
		
	virtual ~XPot();
		

	
	//valuableView
	BHandler*	Handler() { return this;};
	
	
	//events	
	
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage* msg);

	
private:
	
	int			m_vChannel;


};

#endif /* _pot_h */
