/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include <stdio.h>
#include <SupportDefs.h>
#include <SoundPlayer.h>
#include <sndfile.h>
#include <Entry.h>
#include <Sample.h>
#include <Path.h>
#include <samplerate.h>
#include "pitchtable.h"
#include "MediaFileSaver.h"


BSoundPlayer* 		fSoundPlayer = NULL;



#define EXT_SAMPLE_TYPE 0

void PlayBuffer(void* cookie, void* data, size_t size, const media_raw_audio_format& format);

class Note { public:int note;  int getNote(){ return note;}; };

#include "SamplerVoice.h"
#include "MediaFileLoader.h"

status_t
LoadFile(entry_ref *ref, Sample* samp)
{
	BPath	path(ref);
	MediaFileLoader	loader;
	if (loader.Open(path.Path()) != B_OK) {
		return B_ERROR;
	}
	
	const SF_INFO&  sfinfo = loader.Info();
	
	/* SAMPLE SetUp */
	
	samp->channels = sfinfo.channels;
	samp->freq_divisor= 44100.0f / (float)sfinfo.samplerate;

	samp->name.SetTo(path.Leaf());
	samp->path_name.SetTo(path.Path());
	
	samp->fullframes= ceil((float)sfinfo.frames * samp->freq_divisor);
	samp->_totalbytes = samp->fullframes* sizeof(float) * samp->channels;	
	samp->type = EXT_SAMPLE_TYPE;
	
	for (int m = 0 ; m < samp->channels ; m++) {
		samp->wave_data[m] = new float[samp->fullframes];
	}
		
	if (samp->channels == 1)
		samp->wave_data[1] = samp->wave_data[0];

	/******/
	
	int   readcount  = 0;
	int32 position   = 0;
	while ((readcount = loader.ConvertNextBlock()) > 0)
	{	
		float *buf = loader.ConvertFreqBuffer();
		for (int k = 0 ; k < readcount ; k++)
		{
			for (int m = 0 ; m < samp->channels ; m++) {
				samp->wave_data[m][position] = buf[k* samp->channels + m];
			}
			position++;
		};
	};
	
	return B_OK;
}

int
main(int argc, char** argv) {
	
	MediaFileSaver		mFileWriter(1);

	Logger::SetLevel(LOG_LEVEL_TRACE);
	
	TrackSampleRateBuffers	buffers;
	
	if (mFileWriter.Open("output_left.wav") != B_OK){
		printf("Error saving file!\n");
		return 1;
	}

	Sample* sample	   = new Sample();
	entry_ref ref;
	BString	filename("crash.wav");
	if (argc == 2)
		filename.SetTo(argv[1]);
		
	BEntry(filename.String()).GetRef(&ref);
	if (LoadFile(&ref, sample) != B_OK) {
		printf("Error loading file!\n");
		return 1;
	}
	//save in mono
	if (mFileWriter.Status() == B_OK) {
		mFileWriter.WriteBlock(sample->wave_data[0],  sample->fullframes);
	}
	return 0;
}

