/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include <stdio.h>
#include <fluidlite.h>
#include <SupportDefs.h>
#include <SoundPlayer.h>

BSoundPlayer* 		fSoundPlayer = NULL;
fluid_synth_t* 		fSynth = NULL;
fluid_settings_t*	fSettings = NULL;


void PlayBuffer(void* cookie, void* data, size_t size, const media_raw_audio_format& format);
		
void 
_Init(const char* fInstrumentsFile)
{
	int32 fSampleRate = 44100;
	int16 fMaxVoices = 256;
	
	status_t err;
	

	fSettings = new_fluid_settings();
	fluid_settings_setnum(fSettings, (char*)"synth.sample-rate", fSampleRate);
	fluid_settings_setint(fSettings, (char*)"synth.polyphony", fMaxVoices);

	fSynth = new_fluid_synth(fSettings);
	if (!fSynth)
		return;
	
	err = fluid_synth_sfload(fSynth, fInstrumentsFile, 1); 
	if (err < B_OK) {
		fprintf(stderr, "error in fluid_synth_sfload\n");
		return;
	}


	media_raw_audio_format format = media_raw_audio_format::wildcard;
	format.channel_count = 2;
	format.frame_rate = fSampleRate;
	format.format = media_raw_audio_format::B_AUDIO_FLOAT;

	fSoundPlayer = new BSoundPlayer(&format, "FluidLite test", &PlayBuffer, NULL, fSynth);
	err = fSoundPlayer->InitCheck();
	if (err != B_OK) {
		fprintf(stderr, "error in BSoundPlayer\n");
		return;
	}

	fSoundPlayer->SetHasData(true);
	fSoundPlayer->Start();
}
void
PlayBuffer(void* cookie, void* data, size_t size,
		const media_raw_audio_format& format)
{
	fluid_synth_t* synth = (fluid_synth_t*)cookie;


	// we use float samples
	if (synth) {
		fluid_synth_write_float(
			synth, size / sizeof(float) / format.channel_count,
			data, 0, format.channel_count,
			data, 1, format.channel_count);
	}
}


int
main() {
	_Init("example.sf2");
	while (true) {
		sleep(1);
		fluid_synth_noteon(fSynth, 0, 60, 127);
		sleep(1);
		fluid_synth_noteoff(fSynth, 0, 60);
		printf("ping\n");
	}
	return 0;
}

