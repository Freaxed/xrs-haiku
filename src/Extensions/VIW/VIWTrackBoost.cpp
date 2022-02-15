/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "VIWTrackBoost.h"
#include "VIWTrack.h"
#include "filemaker_msg.h"
#include "GfxMsg.h"
#include "TrackManager.h"
#include "viw_locale.h"
#include "PlugWindow.h"
#include "VSTInstrument.h"
#include "VIWPanel.h"
#include "viw_locale.h"

#define	MY_ID	10;

#include <ListView.h>
#include <Path.h>
#include <Message.h>
#include <File.h>
#include <Application.h>
#include "VstManager.h"

/*Annotazioni:
	possibilità di cambiare plug-in on the fly
	il Load and Save devono sempre sapre il nome del plugin (nel messaggio di conf)
	possibilità di istanziare una specialTrack con NULL come path.
	
*/


VIWTrackBoost::VIWTrackBoost():TrackBoost()
{
	list = 	VstManager::Get()->InstrumentVst();
		
	name << T_VIW_NAME;
	id = MY_ID;
	
	panel = new VIWPanel(list);
}

Track*
VIWTrackBoost::getTrack()
{
	return (Track*)(new VIWTrack(this));
}

PlugPanel*
VIWTrackBoost::getPanel()
{
	return panel;
}



VSTPlugin*
VIWTrackBoost::FindVSTi(const char* EffectName)
{
	for(int i=0; i<list->CountItems(); i++){
		VSTPlugin	*stru = (VSTPlugin*)list->ItemAt(i);
		if(strcmp(stru->EffectName(), EffectName) == 0) return stru;
	}
	return NULL;
}
