/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SJFilter.h"

bool
SJFilter::Filter(const entry_ref* ref, BNode* node,
						struct stat_beos* stat, const char* mimeType)
{
	bool rt;
	
	if(node->IsDirectory()) return true;
	
	if(strcmp(mimeType,"audio/XRS-File")==0) rt=true;
		else rt=false;
	
	return rt;
}

