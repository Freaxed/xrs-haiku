/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SJFilter.h"

SJFilter::SJFilter(){}


bool 
SJFilter::Filter(const entry_ref* ref, BNode* node,
						struct stat_beos* stat, const char* mimeType)
						{
						}

/*
bool
SJFilter::Filter(const entry_ref *erf,BNode *node, struct stat_beos *st,const char *filetype)
{
	bool rt;
	
	if(S_ISDIR(st->st_mode)) return true;
	
	if(strcmp(filetype,"audio/XRS-File")==0) rt=true;
		else rt=false;
	
	return rt;
}
*/
