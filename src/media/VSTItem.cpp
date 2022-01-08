/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "VSTItem.h"
#include "VSTParamsView.h"

#include <stdio.h>
#include <stdlib.h>
#include <OS.h>
#include <SupportKit.h>
#include <Mime.h>
#include <Message.h>
#include <Path.h>
#ifdef	VSTMIDI 
	#include <MidiProducer.h>
	#include "VSTMIDIConsumer.h"
#endif
#include "Log.h"
#include "vestige.h"

static int32
VHostCallback(VSTEffect* effect, int32 opcode, int32 index, int32 value,
	void* ptr, float opt);
	

VSTItem::VSTItem(VSTPlugin* templ)
{
	LoadModule(templ->Path(), VHostCallback);
}

VSTItem::~VSTItem()
{
	#ifdef VSTMIDI
		if (fMidiConsumer) {
			sem_id finish = fMidiConsumer->Finish ();
		
			acquire_sem_etc (finish, 1, B_RELATIVE_TIMEOUT, 1000000);
		}
	#endif	
		
		
		LogDebug("Deleting VSTItem.. [%s]\n", EffectName());

}

void
VSTItem::setBPM(int y)
{
	//TODO
}


void VSTItem::SetMidiProducer (int32 id)
{	//QUI__QUI
	#ifdef VSTMIDIX
	BMidiProducer * producer = BMidiRoster::FindProducer (id);
	if (producer)
		fMidiPortName = producer->Name ();
	else
		fMidiPortName = "";
	
	if (producer && fMidiConsumer)
		fMidiConsumer->SetSource (producer);
	else if (producer)
		producer->Release ();
	#endif
}

const char * VSTItem::GetMidiProducerName ()
{
	#ifdef VSTMIDI
		return fMidiPortName.String ();
	#endif
	return "XRSBeta";
	//time
}		

long VSTItem::WantMidi ()
{
	//QUI--QUI
	#ifdef VSTMIDI
	if (fMidiConsumer == 0)
	{
		fMidiConsumer = new MIDI_consumer (this);
		if (fMidiPortName.Length () < 1)
			first_producer_name (fMidiPortName);
		if (fMidiPortName.Length () > 0)
			fMidiConsumer->SetSource (fMidiPortName.String ());
	}
	#endif
	return 1;
	
}

static int32
VHostCallback(VSTEffect* effect, int32 opcode, int32 index, int32 value,
	void* ptr, float opt)
{
	int32 result = 0;
	
	switch(opcode)
	{
		case VST_MASTER_PRODUCT:
			if (ptr) {
				strcpy((char*)ptr, "XRS");
				result = 1;
			}
			break;
		case VST_MASTER_VERSION :
			result = 2000;
			break;
		default:
			LogError("Unhandled VHostCallback opcode %d value %d", opcode, value);
		break;
	}

	return result;
}

/*
long audioMaster (VSTEffect *eff, long opCode, long index, long value, void *ptr, float opt)
{
	long ret = 0;
	//VstTimeInfo *a;
	
	switch (opCode)
	{
		//---------------------------
		case audioMasterAutomate:
			
			break;

		//---------------------------
		case audioMasterVersion:
			ret = 1;
			break;

		//---------------------------
		case audioMasterCurrentId:
			break;

		//---------------------------
		case audioMasterIdle:
			// no need to idle a BeOS host (normaly).
			break;

		//---------------------------
		case audioMasterPinConnected:
			break;

		//---------------------------
		case audioMasterPinConnected+1:// audioMasterMapAsioPorts:
			break;


		//----------------------------------------------------------------------
		// VSTSDK 2.0

		//---------------------------
		case audioMasterWantMidi:
		{
			#ifdef VSTMIDI
			VSTItem * plugin = VSTItem::Identify (eff);
			printf("The Plug In want midi!\n");
			if (plugin)
				ret = plugin->WantMidi ();
			#endif
			break;
		}

		//---------------------------
		case audioMasterGetTime:	// returns const VstTimeInfo* (or 0 if not supported)
									// <value> should contain a mask indicating which fields are required
									// (see valid masks above), as some items may require extensive
									// conversions
		
		//if ((value & kVstTempoValid) && bpm_change)
		{
			
			//time_info.flags |= kVstTempoValid;
			//time_info.tempo =bpm;
			//ret=(long)&time_info;
			//ret=(long)time_info;
			//bpm_change=false;
						
		}
		
									
		//ret= (long)&time_info ;
		
				
		break;

		//---------------------------
		case audioMasterProcessEvents:		// VstEvents* in <ptr>
			break;

		//---------------------------
		case audioMasterSetTime:			// VstTimenfo* in <ptr>, filter in <value>, not supported
			
			
			//a=(VstTimeInfo*)ptr;
			
			break;

		//---------------------------
		case audioMasterTempoAt:			// returns tempo (in bpm * 10000) at sample frame location passed in <value>
			//VSTItem * plugin;
			//plugin = VSTItem::Identify (eff);
			ret=bpm*10000;	
		break;

		//---------------------------
		// parameters
		case audioMasterGetNumAutomatableParameters:
			break;

		//---------------------------
		case audioMasterGetParameterQuantization:	// returns the integer value for +1.0 representation,
											// or 1 if full single float precision is maintained
											// in automation. parameter index in <value> (-1: all, any)
			break;

		//---------------------------
		// connections, configuration
		case audioMasterIOChanged:			// numInputs and/or numOutputs has changed
			break;

		//---------------------------
		case audioMasterNeedIdle:			// plug needs idle calls (outside its editor window)
		{
			LogFatal("VSTItem::audioMasterNeedIdle");
//			VSTItem * plugin = VSTItem::Identify (eff);
//			if (plugin)
//				ret = plugin->NeedIdle ();
			break;
		}

		//---------------------------
		case audioMasterGetSampleRate:
		{
			LogFatal("VSTItem::audioMasterGetSampleRate");
//			VSTItem * plugin = VSTItem::Identify (eff);
//			if (plugin)
//				ret = plugin->GetSampleRate ();
//			else
//				ret = kDefaultFrameRate;
			break;
		}

		//---------------------------
		case audioMasterGetBlockSize:
		{
			LogFatal("VSTItem::audioMasterGetSampleRate");
//			VSTItem * plugin = VSTItem::Identify (eff);
//			if (plugin)
//				ret = plugin->GetBlockSize ();
//			else
//				ret = kDefaultBlockSize;
			break;
		}

		//---------------------------
		case audioMasterGetInputLatency:
			break;

		//---------------------------
		case audioMasterGetOutputLatency:
			break;

		//---------------------------
		case audioMasterGetPreviousPlug:			// input pin in <value> (-1: first to come), returns cEffect*
			break;

		//---------------------------
		case audioMasterGetNextPlug:				// output pin in <value> (-1: first to come), returns cEffect*
			break;

		//---------------------------
		// realtime info
		case audioMasterWillReplaceOrAccumulate:	// returns: 0: not supported, 1: replace, 2: accumulate
			break;

		//---------------------------
		case audioMasterGetCurrentProcessLevel:	// returns: 0: not supported,
										// 1: currently in user thread (gui)
			ret=2;							// 2: currently in audio thread (where process is called)
										// 3: currently in 'sequencer' thread (midi, timer etc)
										// 4: currently offline processing and thus in user thread
										// other: not defined, but probably pre-empting user thread.
			break;

		//---------------------------
		case audioMasterGetAutomationState:		// returns 0: not supported, 1: off, 2:read, 3:write, 4:read/write
			break;

		//---------------------------
		// offline
		case audioMasterOfflineStart:
		case audioMasterOfflineRead:				// ptr points to offline structure, see below. return 0: error, 1 ok
		case audioMasterOfflineWrite:				// same as read
		case audioMasterOfflineGetCurrentPass:
		case audioMasterOfflineGetCurrentMetaPass:
			break;

		//---------------------------
		// other
		case audioMasterSetOutputSampleRate:		// for variable i/o, sample rate in <opt>
			break;

		//---------------------------
		case audioMasterGetSpeakerArrangement:		// (long)input in <value>, output in <ptr>
			break;

		//---------------------------
		case audioMasterGetVendorString:			// fills <ptr> with a string identifying the vendor (max 64 char)
			if (ptr)
			{
				strcpy ((char*)ptr, "Andrea Anzani");
				ret = 1;
			}
			break;

		//---------------------------
		case audioMasterGetProductString:		// fills <ptr> with a string with product name (max 64 char)
			if (ptr)
			{
				strcpy ((char*)ptr, "XRS");
				ret = 1;
			}
			break;

		//---------------------------
		case audioMasterGetVendorVersion:		// returns vendor-specific version
			ret = kAdapterVersion;
			break;

		//---------------------------
		case audioMasterSetIcon:				// void* in <ptr>, format not defined yet
			break;

		//---------------------------
		case audioMasterCanDo:					// string in ptr, see below
		{
			char* text = (char*)ptr;
			//printf ("audioMasterCanDo? %s\n", text);
			if (
				!strcmp (text, "sendVstEvents") ||
				#ifdef VSTMIDI
					!strcmp (text, "sendVstMidiEvent") ||
					!strcmp (text, "sendVstTimeInfo") ||
					!strcmp (text, "receiveVstEvents") ||
					!strcmp (text, "receiveVstMidiEvent") ||
				#endif
				!strcmp (text, "supplyIdle"))
				ret = 1;
			break;
		}

		//---------------------------
		case audioMasterGetLanguage:				// see enum
			ret = kVstLangEnglish;
			break;

		//---------------------------
		case audioMasterSizeWindow:				// index: width, value: height
			break;

		//---------------------------
		case audioMasterVendorSpecific:			// no definition, vendor specific handling
			break;

		//---------------------------
		case audioMasterOpenWindow:				// returns platform specific ptr
		case audioMasterCloseWindow:			// close window, platform specific handle in <ptr>
			break;

		//---------------------------
  		case audioMasterGetDirectory:			// get plug directory, FSSpec on MAC, else char pointer
		{
			LogFatal("VSTItem::audioMasterGetSampleRate");
//			VSTItem * plugin = VSTItem::Identify (eff);
//			if (plugin)
//				//QUI--QUI ret = (long) plugin->fFactory->fFolderPath.String ();
//				printf("Ops.. i don't know where is plug-in directory..\n");
			break;
		}

		case audioMasterUpdateDisplay:			// something has changed, update 'multi-fx' display
			//VSTItem * plugin;
			//plugin = VSTItem::Identify (eff);
			bpm_change=true;
			break;
	}

	return ret;
}
*/

void VSTItem::LoadPreset (BMessage *config)
{	
	VSTEffect* fEffect = Effect();
	const float *	params;
	const void *	chunk;
	ssize_t		size;
	
   	if (fEffect->flags & effFlagsProgramChunks)
	{
		int	prog = 0;
		if (config->FindData ("chunk", B_RAW_TYPE, prog, &chunk, &size) == B_OK)
		{
			fEffect->dispatcher (fEffect, effSetChunk, 0, size, (void*) chunk, 0.f);
		}
	}
	else
	{
		int	prog = 0;
		if(config->FindData ("floats", B_RAW_TYPE, prog, (const void **) &params, &size) == B_OK)
		{
			int	count = (int)size / sizeof (float);
			for (int p = 0; p < count; p++)
			{
				Parameter(p)->SetValue(params[p]);
			}
		}
	}
}


void VSTItem::SavePreset (BMessage *config)
{	
	VSTEffect* fEffect = Effect();
	
	if (fEffect->flags & effFlagsProgramChunks)
	{
		void *		chunk;
		ssize_t		size;
		size = fEffect->dispatcher (fEffect, effGetChunk, 0, 0, &chunk, 0.f);
		config->AddData ("chunk", B_RAW_TYPE, chunk, size);
	}
	else
	{
		int		count = ParametersCount();
		if (count > 0)
		{
			float *	params = new float[count];
			for (int p = 0; p < count; p++)
					params[p] = Parameter (p)->Value();
				config->AddData ("floats", B_RAW_TYPE, params, count * sizeof (float));
			delete[] params;
		}
	}
}
status_t VSTItem::FilterFloat (float **input, float **output, int32 framecount, void *info)
{
	if(!IsActive()) 
		return B_OK;	
	
	if (framecount < BlockSize())
	{
		LogTrace("VSTItem updating block size from %ld to %d", BlockSize(), framecount);		
		SetBlockSize(framecount);
	}
	
	
	//LogError("FilterFloat called\n");
	Effect()->processReplacing (Effect(), input, output, framecount);
	
	//new xrs1.4
	/*
	if (fEffect->flags & effFlagsCanReplacing)
	{
		//if (fEffect->numOutputs < 2)
		//	memcpy (output[1], input[1], sizeof (float) * framecount);
			
		fEffect->processReplacing (fEffect, input, output, framecount);
	}
	else
	{
		for (int out = 0; out < fEffect->numOutputs; out++)	outputs[out] = output[out % 2];
		fEffect->process(fEffect, input, outputs, framecount);
		
	}
	*/
	
	//processing..
	
	//if (fEffect->flags) // & effFlagsIsSynth)
	//{
		//for (int out = 0; out < fEffect->numOutputs; out++)	outputs[out] = output[out % 2];
		//fEffect->process(fEffect, input, outputs, framecount);
		
		
	//}
	
	
	// "normal" plugins :
	
	/*else if (fEffect->flags & effFlagsCanReplacing)
	{
		if(!active )//  dispatcher effGetTailSize
			return B_OK;
		
				
		if (fEffect->numOutputs < 2)
			memcpy (output[1], input[1], sizeof (float) * framecount);
			
		fEffect->processReplacing (fEffect, input, output, framecount);
						
	}
	else
	{
		
		fEffect->process (fEffect, input, output, framecount);
		
	}*/
	
	
	return B_OK;
}

BView * VSTItem::Configure ()
{
	return new VSTParamsView(this);
}


