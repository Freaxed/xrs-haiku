/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <Message.h>
#include <Bitmap.h>

void 	AddIcons(BMessage* msg, BBitmap* largeIcon, BBitmap* miniIcon);	
void 	GetIcons(BBitmap* largeIcon, BBitmap* miniIcon);
void	PrepareMessage(BMessage* msg);
