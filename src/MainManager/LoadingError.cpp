/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "LoadingError.h"
#include "Log.h"

ErrorList LoadingError::mErrors;

void
LoadingError::Add(const char* who, const char* what, const char* solution)
{
	mErrors.AddItem(new ErrorItem(who, what, solution));
	LogError("Error [%s] - [%s] - [%s]", who, what, solution);
}

void
LoadingError::Reset()
{
	
}
