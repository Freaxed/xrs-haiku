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

status_t
InitPlayer(SamplerVoice* v)
{
	int32 fSampleRate = 44100;
	status_t err;
	media_raw_audio_format format = media_raw_audio_format::wildcard;
	format.channel_count = 2;
	format.frame_rate = fSampleRate;
	format.format = media_raw_audio_format::B_AUDIO_FLOAT;
		
	fSoundPlayer = new BSoundPlayer(&format, "SndFile test", &PlayBuffer, NULL, (void*)v);
	err = fSoundPlayer->InitCheck();
	if (err != B_OK) {
		fprintf(stderr, "error in BSoundPlayer\n");
		return err;
	}
	fSoundPlayer->SetVolume(1.0);
	fSoundPlayer->SetHasData(true);
	fSoundPlayer->Start();
	return B_OK;
}


bool played = false;
float amp = 1.0;

int32	
ProcessVoice(SamplerVoice* Voice, float* data, size_t sample_num)
{
//	Note*	curNote		= NULL;	
//	Sample* curSample	= Voice->sample;
	
	if (Voice->is_done()) { return 0; }
	
//	curNote = Voice->n;
	
	uint32 x = 0;
	while(Voice->GetNextFrames(&data[x*2 + 0], &data[x*2 + 1]) && x < sample_num)
	{
		//data[x*2 + 0] *= Left() * amp * curNote->Left();
		//data[x*2 + 1] *= Right()* amp * curNote->Right();
		x++;
	}
	return x;
}


// frame: 	  a single audio value in specific time
// fullframe: a set of audio values from all the channels in specific time
// size:	  frame*channels*sizeof(float)


void
PlayBuffer(void* cookie, void* data, size_t size, const media_raw_audio_format& format)
{
	memset(data, 0x00, size);
	ProcessVoice((SamplerVoice*)(cookie), (float*)data, size / (2*sizeof(float)));
}


int
main(int argc, char** argv) {
	
	Logger::SetLevel(LOG_LEVEL_TRACE);
	
	TrackSampleRateBuffers	buffers;

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
	
	Note note;
	note.note = 60; //C-5!
	SamplerVoice* v    = new SamplerVoice(&note, sample, buffers);
	v->loop 	= false; // working!
	v->reverse  = false;  // working!!
	
	if (InitPlayer(v) != B_OK){
		printf("Error init player\n");
	}
	while (!v->is_done()) {
		sleep(1);
		printf("ping\n");
	}
	printf("stop\n");
	fSoundPlayer->SetHasData(false);
	fSoundPlayer->Stop();


	v->reverse  = true;
	v->ResetPosition();
	fSoundPlayer->SetHasData(true);
	fSoundPlayer->Start();
	while (!v->is_done()) {
		sleep(1);
		printf("ping\n");
	}
	printf("stop\n");
	fSoundPlayer->SetHasData(false);
	fSoundPlayer->Stop();
	

	
	v->reverse  = false;
	v->loop = true;
	v->ResetPosition();
	fSoundPlayer->SetHasData(true);
	fSoundPlayer->Start();
	int loop = 5;
	while (loop--) {
		sleep(1);
		printf("ping %d)\n", loop);
	}
	printf("stop\n");
	fSoundPlayer->SetHasData(false);
	fSoundPlayer->Stop();
	delete fSoundPlayer;
	return 0;
}

