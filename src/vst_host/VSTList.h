/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 * Freely based on vst_host for haiku done by Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 */


#ifndef __VST_LIST_H__
#define __VST_LIST_H__

#include "VSTHost.h"

class VSTList  {
public:
	virtual		 			~VSTList();
	         				 VSTList();
	virtual		int32 		CountPlugins();
	            VSTPlugin*  GetPluginAt(int32 pos) { return (pos >=  fPluginsList.CountItems()) ? NULL : (VSTPlugin*)fPluginsList.ItemAt(pos); }
	  			VSTPlugin*  InstantiatePlugin(VSTPlugin* plugin);
private:	
				int 		ScanPluginsFolder(const char* path, bool make_dir = false);
				BList		fPluginsList;
				

};

#endif //__VST_LIST_H__
