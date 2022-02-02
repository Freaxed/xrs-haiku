/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _VSTInstrument_H_
#define _VSTInstrument_H_

#include "VSTItem.h"
#include "vestige.h"

class VSTInstrumentPlugin :  public VSTItem
{
public:

				VSTInstrumentPlugin(VSTPlugin* plug);
				~VSTInstrumentPlugin();
	
	void		SendNote(long,float,bool on=true);
	void		StopAll();

private:
		
	VstMidiEvent	*stopall_event;
	VstEvents		*stopall_list;
	
	
	VstEvents		*note_list;
	VstMidiEvent	*noteon_event;	//start note
	VstMidiEvent	*noteoff_event;	//stop nte
	
};
#endif 
