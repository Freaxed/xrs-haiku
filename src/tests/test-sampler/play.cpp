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

BSoundPlayer* 		fSoundPlayer = NULL;



#define LogInfo(X...)    { printf(X); printf("\n"); }
#define LogError(X...)   { printf(X); printf("\n"); }
#define LogDetail(X...)  { printf(X); printf("\n"); }

#define EXT_SAMPLE_TYPE 0

void PlayBuffer(void* cookie, void* data, size_t size, const media_raw_audio_format& format);


Sample* sample = new Sample();

struct Voice {
	Voice() { position = 0; reverse = false; played = false; loop = false;}
//private:
	uint32 	position; //in fullframe
	bool	played; //??
// public:	

	bool	reverse; //is this a voice property? or a sample property?
	bool	loop;
	uint32	get_position(uint32 fullframes)  //in fullframes
	{		
		return reverse ? (fullframes - 1) - position : position;
	}
};

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
	
	samp->frames = ceil((float)sfinfo.frames * samp->freq_divisor);
	samp->_totalbytes = samp->frames * sizeof(float) * samp->channels;	
	samp->type = EXT_SAMPLE_TYPE;
	
	for (int m = 0 ; m < samp->channels ; m++) {
		samp->wave_data[m] = new float[samp->frames];
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
InitPlayer(Voice* v)
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

	fSoundPlayer->SetHasData(true);
	fSoundPlayer->Start();
	return B_OK;
}

int
GetNextDataFromVoice(Voice* v, Sample* sample, float* left, float* right)
{
	uint32 fullframes = (sample->frames);// / sample->channels);
	if (v->played) {
		*left = *right = 0.0f;
		return 0;
	}
	
	//printf("Debug reverse: %d -> %d \n", v->position, v->reversePosition(sample->frames / sample->channels));
	
	*left   = sample->wave_data[0][v->get_position(fullframes)];
	*right  = sample->wave_data[1][v->get_position(fullframes)];
	v->position++;
	
	if(v->position >= fullframes) {
		printf("played!\n");
		v->played = !v->loop;
		v->position = 0;
	}
		
	return 0;
}

// frame: 	  a single audio value in specific time
// fullframe: a set of audio values from all the channels in specific time
// size:	  frame*channels*sizeof(float)


void
PlayBuffer(void* cookie, void* data, size_t size, const media_raw_audio_format& format)
{
	Voice* v  = (Voice*)(cookie);
	uint32 fullframes = sample->frames;// / sample->channels;
	uint32 length = MIN(fullframes-v->position, size/ (2*sizeof(float)));
	
	float* dest = (float*)data;
	for(uint32 z=0; z<length; z++)	{
		GetNextDataFromVoice(v, sample, &dest[z*2 + 0], &dest[z*2 + 1]);
	}
}


int
main(int argc, char** argv) {
	
	Voice* v    = new Voice();
	v->loop 	= false; // working!
	v->reverse  = false; // working!!
	
	
	entry_ref ref;
	BString	filename("crash.wav");
	if (argc == 2)
		filename.SetTo(argv[1]);
		
	BEntry(filename.String()).GetRef(&ref);
	if (LoadFile(&ref, sample) != B_OK) {
		printf("Error loading file!\n");
		return 1;
	}
	if (InitPlayer(v) != B_OK){
		printf("Error init player\n");
	}
	while (!v->played) {
		sleep(1);
		printf("ping\n");
	}
	printf("stop\n");
	fSoundPlayer->Stop();
	delete fSoundPlayer;
	return 0;
}

