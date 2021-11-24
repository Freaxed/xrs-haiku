/*
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "IconListItem.h"
#include <Entry.h>
#include <Bitmap.h>

class XListItem : public IconListItem
{
	public:
		XListItem(BBitmap *bp, entry_ref*);
		entry_ref	getRef();
		
	private:		
		entry_ref	fRef;
}; 
