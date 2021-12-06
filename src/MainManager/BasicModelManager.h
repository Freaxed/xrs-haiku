/*
 * Copyright 2018, Your Name <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _BASICMODEL_MANAGER_H
#define _BASICMODEL_MANAGER_H


#include <SupportDefs.h>

#include "IntValuable.h"



class BasicModelManager{
public:

		BasicModelManager();
		~BasicModelManager();
		
private:
	GenericValuable<int16>*	fVBpm;
	
};


#endif // _BASICMODEL_MANAGER_H
