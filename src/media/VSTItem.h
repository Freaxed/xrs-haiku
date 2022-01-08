/*

	VSTItem.h
	by Andrea Anzani

	Original code by Georges-Edouard Berenger
	
*/

#ifndef _VSTItem_H_
#define _VSTItem_H_

#include "VSTHost.h"

#include <Entry.h>
#include <KernelKit.h>
#include <Locker.h>
#include <String.h>
#include <Entry.h>
#include <string.h>
#include <List.h>

#ifdef VSTMIDI
	class MIDI_consumer;
#endif


const int kDefaultBlockSize = 4096;
const int kDefaultFrameRate = 44100;
const int kAdapterVersion   = 0x0100;

#define VST_INSTRUMENTS 1
#define VST_INSTRUMENTS_MAX_OUTPUT 16

class BView;
class BMessage;

class VSTItem : public VSTPlugin
{

public:

					   VSTItem(VSTPlugin* templ);
		 	virtual	  ~VSTItem();
			
	virtual 	BView *		Configure ();
	 		
	 		status_t		FilterFloat (float **input, float **output, int32 framecount, void *info);
	 		
			void			SavePreset(BMessage *);
			void			LoadPreset(BMessage *);
			void 			setBPM(int);
			long			WantMidi ();
			void			SetMidiProducer (int32 id);
	const char *			GetMidiProducerName ();
		
protected: 
	
	#ifdef VSTMIDI
		MIDI_consumer *		fMidiConsumer;
		BString				fMidiPortName;
	#endif

		float*				outputs[VST_INSTRUMENTS_MAX_OUTPUT];
};



#endif
