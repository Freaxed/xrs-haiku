/*
  An example of how to use FluidSynth.
  To compile it on Linux:
  $ gcc -o example example.c `pkg-config fluidsynth --libs`
  To compile it on Windows:
    ...
  Author: Peter Hanappe.
  This code is in the public domain. Use it as you like.
*/
#include <fluidsynth.h>
#if defined(WIN32)
#include <windows.h>
#define sleep(_t) Sleep(_t * 1000)
#include <process.h>
#define getpid _getpid
#else
#include <stdlib.h>
#include <unistd.h>
#endif


void show_settings(void *data, const char *name, int type) {

	fluid_settings_t* settings = (fluid_settings_t*)data;
	printf("Setting: [%s] ", name);
	switch(type){
	case 0: {
		double val;
		fluid_settings_getnum(settings, name, &val);
		printf("double [%lf]\n", val); 
	}
		break;
	case 2: {
		char* val = NULL;
		fluid_settings_dupstr(settings, name, &val);
		printf("string [%s]\n", val); 
		fluid_free(val);
	}
		break;
	case 1: {
		int val = 0;
		fluid_settings_getint(settings, name, &val);
		printf("integer [%d]\n", val); 
	}
		break;
	case 3: printf("sets \n"); break;
	default:
		printf(" ?? \n"); break;
	};
}
	
int main(int argc, char **argv)
{
	float *l = new float[1024];
	float* r = new float[1024];
	l[512] = 0;
	r[512] = 0;
	
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    fluid_audio_driver_t *adriver;
    fluid_sfont_t* sfont;
    int sfont_id;
    int i, key;
    /* Create the settings. */
    settings = new_fluid_settings();
    fluid_settings_setnum(settings, "synth.gain", 1.0);
//	fluid_settings_setint(settings, "synth.polyphony", 256);
	fluid_settings_setint(settings, "synth.midi-channels", 16);
	fluid_settings_setstr(settings, "audio.driver", "");
	fluid_settings_setnum(settings, "synth.sample-rate", 44100.0);
	
	fluid_settings_foreach(settings, settings, show_settings);
    /* Change the settings if necessary*/
    /* Create the synthesizer. */
    synth = new_fluid_synth(settings);
    /* Create the audio driver. The synthesizer starts playing as soon
       as the driver is created. */
    //adriver = new_fluid_audio_driver(settings, synth);
    /* Load a SoundFont and reset presets (so that new instruments
     * get used from the SoundFont) */
    sfont_id = fluid_synth_sfload(synth, "example.sf2", 1);
    if(sfont_id == FLUID_FAILED)
    {
        puts("Loading the SoundFont failed!");
        goto err;
    
    }
    
	sfont = fluid_synth_get_sfont_by_id(synth, sfont_id);
    printf("Soundfont name: %s\n", fluid_sfont_get_name(sfont));
   
    /* Initialize the random number generator */
    srand(getpid());
    for(i = 0; i < 12; i++)
    {
        /* Generate a random key */
        key = 60 + (int)(12.0f * rand() / (float) RAND_MAX);
        /* Play a note */
        printf("playing note %d\n", key);
        fluid_synth_noteon(synth, 0, key, 80);
        /* Sleep for 1 second */
        int y = 0;
        for (y=0;y< 16;y++) {
        	if (fluid_synth_write_float(synth, 1024, l, 0, 1, r, 0, 1) != FLUID_OK)
        		printf("ERROR!\n");
        	if (l[512] != 0) printf("%d - l = %f\n", y, l[512]);
        	l[512] = 0;
        }
        sleep(1);
        /* Stop the note */
        fluid_synth_noteoff(synth, 0, key);
    }
err:
    /* Clean up */
    //delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return 0;
}
