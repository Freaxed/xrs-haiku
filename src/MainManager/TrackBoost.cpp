/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "TrackBoost.h"
#include "PlugPanel.h"
#include "TrackManager.h"

#include <Application.h>
#include <File.h>


TrackBoost::TrackBoost(){}

int64
TrackBoost::ReadFixed(int64 *data,BFile* file)
{
	int64 code;
	file->Read(&code,sizeof(int64));
	file->Read(data,sizeof(int64));
	return code;	
}

Track*
TrackBoost::getCurrentTrack(){ return TrackManager::Get()->getCurrentTrack();}


