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

#include "VSTList.h"

VSTList::VSTList()
{
	fPluginsList.MakeEmpty();

	BStringList folders;

	BPathFinder::FindPaths(B_FIND_PATH_ADD_ONS_DIRECTORY,
		"media/vstplugins/", B_FIND_PATH_EXISTING_ONLY, folders);

	for (int32 i = 0; i < folders.CountStrings(); i++)
		ScanPluginsFolder(folders.StringAt(i).String());
		
	ScanPluginsFolder("/boot/data/haikuports/media-plugins/open303/Out");
}

VSTList::~VSTList()
{	
	
}
	
	
int32 
VSTList::CountPlugins()
{
	return fPluginsList.CountItems();
}
/*
status_t
VSTList::GetFlavorAt(int32 idx, const flavor_info** info) 
{
	if (idx < 0 || idx >= fPluginsList.CountItems())
		return B_ERROR;
		
	VSTPlugin *plugin = (VSTPlugin*)fPluginsList.ItemAt(idx);	
	
	flavor_info *f_info = new flavor_info;
	f_info->internal_id = idx;
	f_info->kinds = B_BUFFER_CONSUMER | B_BUFFER_PRODUCER | B_CONTROLLABLE;
	f_info->possible_count = 0;
	f_info->flavor_flags = 0;
	f_info->name = plugin->ModuleName();
	f_info->info = plugin->Product();
	
	media_format *format = new media_format;
	format->type = B_MEDIA_RAW_AUDIO;
	format->u.raw_audio = media_raw_audio_format::wildcard;
	format->u.raw_audio.format = media_raw_audio_format::B_AUDIO_FLOAT;
	
	f_info->in_format_count = 1;
	f_info->in_formats = format;

	format = new media_format;
	format->type = B_MEDIA_RAW_AUDIO;
	format->u.raw_audio = media_raw_audio_format::wildcard;
	format->u.raw_audio.format = media_raw_audio_format::B_AUDIO_FLOAT;

	f_info->out_format_count = 1;
	f_info->out_formats = format;		
	
	*info = f_info;
	
	return B_OK;
}
*/
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
			} else
				delete plugin;
		}
	}
	return 0;
}
