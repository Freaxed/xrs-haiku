/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 * Freely based on vst_host for haiku done by Gerasim Troeglazov (3dEyes**), 3dEyes@gmail.com.
 */



#include <Entry.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <PathFinder.h>
#include <StringList.h>
#include "Log.h"
#include "VSTList.h"

VSTList::VSTList()
{
	fPluginsList.MakeEmpty();

	BStringList folders;

	BPathFinder::FindPaths(B_FIND_PATH_ADD_ONS_DIRECTORY,
		"media/vstplugins/", B_FIND_PATH_EXISTING_ONLY, folders);

	for (int32 i = 0; i < folders.CountStrings(); i++)
		ScanPluginsFolder(folders.StringAt(i).String());
}

VSTList::~VSTList()
{	
	
}
	
	
int32 
VSTList::CountPlugins()
{
	return fPluginsList.CountItems();
}

VSTPlugin* 
VSTList::InstantiatePlugin(VSTPlugin* plugin)
{
	VSTPlugin* new_plugin = new VSTPlugin();
	new_plugin->LoadModule(plugin->Path());
	return new_plugin;
}

int
VSTList::ScanPluginsFolder(const char* path, bool make_dir)
{
	BEntry ent;

	BDirectory dir(path);
	if (dir.InitCheck() != B_OK) {
		if (make_dir == true)
			create_directory(path, 0755);
		return 0;
	}

	while(dir.GetNextEntry(&ent) == B_OK) {
		BPath p(&ent);
		if (ent.IsDirectory()) {
			ScanPluginsFolder(p.Path());
		} else {
			VSTPlugin* plugin = new VSTPlugin();
			int ret = plugin->LoadModule(p.Path());
			if (ret == B_OK) {
				plugin->UnLoadModule();
				fPluginsList.AddItem(plugin);				
			} else {
				LogError("Can't load plugin %s (%d)", p.Path(), ret);
				delete plugin;
			}
		}
	}
	return 0;
}
