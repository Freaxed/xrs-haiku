/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <FilePanel.h>
#include <string.h>

class SJFilter : public BRefFilter
{
public:
		SJFilter();
		
		bool Filter(const entry_ref* ref, BNode* node,
						struct stat_beos* stat, const char* mimeType);

//	virtual bool Filter(const entry_ref *erf,BNode *node,struct stat_beos *st,const char *filetype);
};
