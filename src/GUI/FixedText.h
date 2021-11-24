/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef FixedText_H_
#define FixedText_H_

#include <StringView.h>
#include <String.h>

class FixedText : public BStringView {

	public:
				 FixedText(BRect rect,const char* text);

};

#endif
//--
