/*
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "XListItem.h"

#include <string.h>
#include <malloc.h>
#include <Message.h>


XListItem::XListItem(BBitmap *bp, entry_ref *ref):IconListItem(bp,ref->name, 0, false)
{
	fRef = *ref;
}

entry_ref
XListItem::getRef()
{
	return fRef;	
}
