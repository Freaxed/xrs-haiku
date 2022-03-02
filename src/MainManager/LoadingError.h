/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef LoadingError_H
#define LoadingError_H


#include <SupportDefs.h>
#include <ObjectList.h>
#include <String.h>

struct ErrorItem {
	
	ErrorItem(const char* who, const char* what, const char* solution) {
		this->who.SetTo(who);
		this->what.SetTo(what);
		this->solution.SetTo(solution);
	}
	
	BString	who;
	BString	what;
	BString solution;
};

typedef BObjectList<ErrorItem> ErrorList;

class LoadingError {

public:
	static void Add(const char* who, const char* what, const char* solution);
	static void	Reset();
	
	static const ErrorList&	GetErrorList() { return mErrors;}

private:

	static ErrorList	mErrors;

};


#endif // LoadingError.h_H
