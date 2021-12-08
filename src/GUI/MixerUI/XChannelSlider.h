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
							int channel,
							BMessage * model,int min,int max,
							orientation o);
											
	
		BHandler*	Handler() { return this; }
		//events	
		void AttachedToWindow();
		void DetachedFromWindow();
		void MessageReceived(BMessage* msg);
		
		BHandler* GetHandler() { return this;}
		 
		void 	DrawThumb();
		
	private:
		const BBitmap*	fThumb;
		ValuableID	vID;

};

#endif
//.
