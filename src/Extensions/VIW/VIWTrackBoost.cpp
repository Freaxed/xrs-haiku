/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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
	VIWTrack *trk=new VIWTrack();
	return (Track*)(trk);
}

PlugPanel*
VIWTrackBoost::getPanel()
{
	return panel;
}


void
VIWTrackBoost::LoadTrackSettings(Track* trk, BMessage* data){
/*		
		VIWTrack*	st=(VIWTrack*)trk;
		BMessage*		mes=new BMessage();
		
		BString name;
		status_t stat=data->FindString("plugin_name",&name);
				
		if(stat==B_OK){
		
		
		
		int pos=FindVSTi(name.String());
		
		if(pos<0) {
		
		BString error_s;
		error_s.SetTo(T_VIW_CANT_FIND);
		error_s << name.String() <<" \n";
		data->AddString("error",error_s.String());
		//printf("%s",error_s.String());
		return; // VSTi NOT FOUND!
		}
		PlugInEntry*	stru=(PlugInEntry*)list->ItemAt(pos);
		
		st->LoadVSTi(stru->ref.name);
		
		
		if(data->FindMessage("instrument_preset",mes)==B_OK)
		{
			if(st->getViw()) st->getViw()->LoadPreset(mes);
		}
		else
		if(data->FindMessage("instrument_preset_old",mes)==B_OK)
		{
			if(st->getViw()) st->getViw()->LoadPresetOLD(mes);
		}
		
		mes->MakeEmpty();
		data->FindMessage("window_settings",mes);
		
		if(st->getWin()) st->getWin()->LoadPref(mes);
		}
*/
}
void			
VIWTrackBoost::SaveTrackSettings(Track* trk,BMessage* data)
{/*
		VIWTrack*	st=(VIWTrack*)trk;
		BMessage*		mes;
		
		if(st->getViw()!=NULL){
			mes=new BMessage();
			entry_ref ref;
			get_ref_for_path(st->getViw()->filename_path.String(),&ref);
			data->AddString("plugin_name",ref.name);
						
			st->getViw()->SavePreset(mes);
			data->AddMessage("instrument_preset",mes);
			
			//mes->PrintToStream();
			
			mes->MakeEmpty();
			
			st->getWin()->SavePref(mes);
			data->AddMessage("window_settings",mes);	
			
		}
	*/
}

int
VIWTrackBoost::FindVSTi(const char* name)
{/*
	BPath path;
	for(int i=0;i<list->CountItems();i++){
			PlugInEntry*	stru=(PlugInEntry*)list->ItemAt(i);
			path.SetTo(stru->ref.name);
			path.InitCheck();
			//printf("ref.name %s\n",path.Leaf());
			if(strcmp(name,path.Leaf())==0) return i;
	}
*/
	return -1;
}
