/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#ifndef _XChannelSlider_
#define _XChannelSlider_

#include "AChannelSlider.h"
#include "ValuableManager.h"


class XChannelSlider: public AChannelSlider, public ValuableReceiver
{
	public:
		   XChannelSlider(	BRect area,
							const char * valuablename,
							ValuableID id,
							orientation o);
	
		//events	
		void AttachedToWindow();
		void DetachedFromWindow();
		void MessageReceived(BMessage* msg);
		
		BHandler* GetHandler() { return this;}
		 
	
	private:
		ValuableID	vID;

};

#endif
//.
