/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _CONFIGURATOR_H_
#define _CONFIGURATOR_H_

#include <Message.h>
#include <File.h>
#include <Path.h>

class Configurator : public BMessage
{
	
public:
		static Configurator* Get();
 	
		 void		Init(const char*);
		 void   	Close();
		 
		 float		FloatKey(const char*,float); //same as GetFloat
		 
		 void		PutFloatKey(const char*,float);

private:

				Configurator();
 	BPath		fPath;

};

Configurator* Config();


#endif


