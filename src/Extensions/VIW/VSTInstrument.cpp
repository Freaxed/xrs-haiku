/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "VSTInstrument.h"
#include <stdio.h>

VSTInstrumentPlugin::VSTInstrumentPlugin (const char *filename):VSTItem(filename)
{
	if (InitCheck()==B_OK)
	{
				
		//StopAll
		
		stopall_list=new VstEvents();
		stopall_list->numEvents=1;
		
		stopall_event=new VstMidiEvent();
		stopall_event->type=kVstMidiType;
		stopall_event->byteSize=24;
		stopall_event->deltaFrames=0;
		stopall_event->noteLength=0;
		stopall_event->noteOffset=0;
		
		stopall_event->midiData[2]=0x00; 		//
		stopall_event->midiData[1]=(char)123;	//	Message: AllNoteOff
		stopall_event->midiData[0]=0xB0;  		//
		
		//stopall_event->midiData[2]=0x00; 		//
		//stopall_event->midiData[1]=0x00; 		//(char)120;//	Message:: AllSoundsOff
		//stopall_event->midiData[0]=0x00; 		//0xB0;		 

		stopall_list->events[0]=(VstEvent*)stopall_event;


		// Note On/Off
		
		noteon_event=new VstMidiEvent();

		noteon_event->type=kVstMidiType;
		noteon_event->byteSize=24;
		noteon_event->deltaFrames=0;
		noteon_event->noteLength=0;
		noteon_event->noteOffset=0;

		noteoff_event=new VstMidiEvent();

		noteoff_event->type=kVstMidiType;
		noteoff_event->byteSize=24;
		noteoff_event->deltaFrames=0;
		noteoff_event->noteLength=0;
		noteoff_event->noteOffset=0;

		note_list=new VstEvents();
		note_list->numEvents=2;
		
		note_list->events[0]=(VstEvent*)noteoff_event;
		note_list->events[1]=(VstEvent*)noteon_event;
	}
}

void
VSTInstrumentPlugin::StopAll()
{
	//((AudioEffectX*)fEffect->object)->processEvents(stopall_list);
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0,(void*)stopall_list, 0.);
}
void
VSTInstrumentPlugin::SendNote(long note,float vel,bool on)
{
	
	if(noteon_event->midiData[0]!=0x00)
	{
		noteoff_event->midiData[2]=noteon_event->midiData[2]; 	//Volume = 0x7f=255=MAX;
		noteoff_event->midiData[1]=noteon_event->midiData[1];	//
		noteoff_event->midiData[0]=0x80;  						// noteoff of the previus note
	}
	
	
	noteon_event->midiData[2]=(char)(vel*255.); 	//Volume = 0x7f=255=MAX;
	noteon_event->midiData[1]=(char)note;		
	
	if(on) 
		noteon_event->midiData[0]=0x90;  		//note_on on the first channel (0);
	else
		noteon_event->midiData[0]=0x80;		// use 0x80 for note_off on the first channel (0);
	
	fEffect->dispatcher (fEffect, effProcessEvents, 0, 0,(void*)note_list, 0.);
}

