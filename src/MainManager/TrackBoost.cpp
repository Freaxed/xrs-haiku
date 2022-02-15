/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "TrackBoost.h"
#include "TrackManager.h"

TrackBoost::TrackBoost(){}

Track*
TrackBoost::getCurrentTrack(){ return TrackManager::Get()->getCurrentTrack();}


