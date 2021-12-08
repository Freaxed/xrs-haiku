/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#ifndef _EX_
#define _EX_

#include "ValuableManager.h"
#include <Looper.h>

class GenericReceiver : public BLooper, public ValuableReceiver {
	public:
	GenericReceiver():BLooper("Generic"){};
	
	void MessageReceived(BMessage* msg);
	
	BHandler* GetHandler() {
		return this;
	}
	
};


#endif

