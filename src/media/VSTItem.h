/*

	VSTItem.h
	by Andrea Anzani

	Original code by Georges-Edouard Berenger
	
*/

#ifndef _VSTItem_H_
#define _VSTItem_H_

#ifndef __audioeffectx__
	#include "audioeffectx.h"
#endif


#include <Entry.h>
#include <KernelKit.h>
#include <Locker.h>
#include <String.h>
#include <Entry.h>
#include <string.h>
#include <List.h>

//#define VSTMIDI

#ifdef VSTMIDI
	class MIDI_consumer;
#endif

typedef	struct PlugInEntry {
		BString			name;
		BString			product;
		entry_ref		ref;	
		bool			isSynth;
};

class VSTItem;

typedef struct {
	AEffect	*			effect;
	VSTItem *			plugin;

} effectPluginPair;

const int kDefaultBlockSize = 4096;
const int kDefaultFrameRate = 44100;
const int kAdapterVersion = 0x0100;

#define VST_INSTRUMENTS 1
#define VST_INSTRUMENTS_MAX_OUTPUT 16

class BView;
class BMessage;

class VSTItem
{

public:

					   VSTItem(const char *name);
		 	virtual	  ~VSTItem();
   
    		status_t	InitCheck () { return fImage > 0 ? B_OK : B_ERROR; }
			
	virtual 	BView *		Configure ();
	 		
	 		status_t	FilterFloat (float **input, float **output, int32 framecount, void *info);
			void			SavePreset(BMessage *);
			void			LoadPreset(BMessage *);
			//deprecated (only for 1.2 compatib.)
			void			LoadPresetOLD(BMessage *);
			
	static	void 			load_plug_ins (const char *path); //, BList*);
			
			void 			setBPM(int);
			
			void			SetStatus(bool b);
			bool			GetStatus() {return active;}
			
			long			NeedIdle ();
			void			IdleLoop ();
			long			WantMidi ();

	
	static 	void				Register (VSTItem * factory, AEffect * effect);
	static 	void				Unregister (AEffect * effect);
	static 	VSTItem		 	*Identify (AEffect * effect);
			int32			GetSampleRate () { return fSampleRate; }
			int32			GetBlockSize () { return fBlockSize; }
			uint32			GetLastFilterTime () { return fLastFilter; }


	AEffect *			fEffect;
	BString				name;
	BString				filename_path;
	BString				presets_path;
	BString				productname;
	
	
	void					SetMidiProducer (int32 id);
	const char *			GetMidiProducerName ();
		
protected: 
	
	
	static 	effectPluginPair*	fPairs;
	static 	int					fPairsCount;
	static	BLocker				fLock;
			int32				fSampleRate;
			int32				fBlockSize;
			uint32				fLastFilter;
	bool							active;
	#ifdef VSTMIDI
		MIDI_consumer *		fMidiConsumer;
		BString				fMidiPortName;
	#endif

			image_id			fImage;
			thread_id			fIdleThread;
			sem_id				fIdleSem;
			float*				outputs[VST_INSTRUMENTS_MAX_OUTPUT];
			


};



long audioMaster (AEffect *eff, long opcode, long index, long value, void *ptr, float opt);
void certify_effect (AEffect * effect);

#endif
