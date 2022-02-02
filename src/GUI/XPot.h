/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _XPot_h
#define _XPot_h

#include 	"APot.h"
#include 	"ValuableManager.h"

#define	SB_MSG	'sbmp'

class XPot : public APot
{
public:
	XPot(BRect frame, const char *name,
		ValuableID id, int32 min, int32 max,
		BBitmap *p1 = NULL ,BBitmap *p2 = NULL);

	XPot(const char *name,
		ValuableID id, int32 min, int32 max,
		BBitmap *p1 = NULL ,BBitmap *p2 = NULL);
		
	virtual ~XPot();

	
	//events	
	
	void AttachedToWindow();
	void DetachedFromWindow();
	void MessageReceived(BMessage* msg);

	
private:
	ValuableID vID;


};

#endif /* _pot_h */
