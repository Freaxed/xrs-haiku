/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TRACK_ITEM_
#define _TRACK_ITEM_

#include "NameItem.h"

#include <InterfaceKit.h>
#include <String.h>

class BBitmap;

class TrackItem : public BStringItem {

public:
						TrackItem(const char*,BView* ow);
			void		ChangeStatus();
			bool		GetStatus(){ return active;};
			void		SetStatus(bool b){ active=b;}
			void		DrawItem(BView *owner,BRect bounds,bool complete = false) ;
			void 		Update(BView *owner, const BFont *font);	
		
			void 		Reset(NameItem*,NameItem* p2=NULL);
private:			
				bool 		active;
				NameItem* 	pad;
				NameItem*	pad2;
				BView*		part;
				BView*		owner;
				BBitmap*	title;
};

#endif
