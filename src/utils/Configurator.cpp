/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Configurator.h"
#include "version.h"

#include <stdio.h>
#include <FindDirectory.h>
#include <string.h>
#include <Entry.h>
#include "Log.h"

Configurator* Config(){ 
	return Configurator::Get();
}


Configurator* 
Configurator::Get(){
	static Configurator*	instance = NULL;
	if (instance == NULL) 
			instance=new Configurator();
	return instance;
}
	
Configurator::Configurator(){}						

void
Configurator::Init(const char *filename)
{
	find_directory (B_USER_SETTINGS_DIRECTORY, &Config()->fPath, true);
	fPath.Append (filename);
	

	BFile loadFile(Config()->fPath.Path (), B_READ_WRITE | B_CREATE_FILE);
	if (loadFile.InitCheck () == B_OK)
	{
		Unflatten(&loadFile);
	}
}

void 
Configurator::Close()
{

	LogInfo("Writing configuration file!");
	
	RemoveName("Version");
	AddString("Version", VERSION_NUMBER);
	
	BFile cf_file(fPath.Path(),B_WRITE_ONLY|B_ERASE_FILE);
	
	if (Flatten(&cf_file) != B_OK) 
		LogError("Error in writing the configuration to file!\n");
}

float
Configurator::
FloatKey(const char *key,float def)
{
	return GetFloat(key, def);
}

void
Configurator::
PutFloatKey(const char *key,float def)
{
	RemoveName(key);
	AddFloat(key,def);
	LogTrace("Configurator:PutFloatKey(%s, %f)",key,def);
}

