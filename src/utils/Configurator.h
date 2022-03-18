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

class Configurator
{

public:
		static Configurator* Get();
 	
		 void		cf_Init(const char*);
		 void   	cf_Close();
		 
		 float		FloatKey(const char*,float);
		 
		 void		PutFloatKey(const char*,float);
		 
		 void		PutString(const char*,const char*);
		 
		 void		String(const char*,const char*,const char*,int);
		 
		 status_t	Ref(const char *key,entry_ref *ref,const char *def);
		 
		 void		PutRef(const char *key,entry_ref *ref);
		 
		 void		ReplaceRef(const char *key,entry_ref *ref);


private:

				Configurator();	

 	BMessage*	cf_in_msg;
 	BMessage*	cf_out_msg;
 	BFile*		cf_file;
 	BPath		cf_path;

};

Configurator* Config();


#endif


