/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TNTrack.h"
#include "Note.h"
#include <math.h>
#include <stdio.h>
#include "defaults.h"


TNTrack::TNTrack():Track() , output(&vco) 
{
	vco.SetOutput(&vcf);
	vcf.SetOutput(&vca);
	vca.SetOutput(&dly);

//	output = &vco;

	tuneoffset = 0;

	// Tempo changes:
	note_length 		= 0;
	current_note_length = 0;

	ResetName();
}

int	
TNTrack::getModel()
{	
	return 1; 
}


void
TNTrack::Process(float** buffer,int32 samples_num,int spiaz,float factor)
{
	if(curNote==NULL) return;
	
	if(current_note_length<=0 && !(curNote->getOct()&8) ) stopVoice();
	
	output->Process(NULL, samples_num,1);
		
	float *f=dly.getBuffer();
	
	if(!f) return;

	for(int y=0;y<samples_num;y++)
	{
		buffer[0][spiaz+y]+=f[y]/32767.0f * Left()  * curNote->Left()   * PUMP_UP_FACTOR * factor;
		buffer[1][spiaz+y]+=f[y]/32767.0f * Right() * curNote->Right()  * PUMP_UP_FACTOR * factor;
	}
	current_note_length-=samples_num;
	return;
}


//int min(int a, int b) { return a<b ? a:b; }

void 
TNTrack::Message(SynthMessage msg, float data)
{
	switch(msg) 
	{
		case TempoChange: 
			note_length=(int32)data;
			break;
		case SystemStop:
			stopVoice(); Reset();
			break;
		case SystemReset:
			Reset();
			break;
		default: 
			break;
	}
	
	if(output) 
		output->Message(msg, data);
}

// convert halftone number into frequency.  halftone 93 is concert A (440Hz).

static float notefreq(int16 halftone)
{
	return 55.0f * powf(2, (float)(halftone-57)/12.0f);
}


XRSVoice
TNTrack::newVoice(Note* n,int VoiceTag)
{ 
	curNote = n;
	current_note_length=note_length/8;
	output->Message(NoteChange, notefreq(tuneoffset + (int16)curNote->getNote()));
	output->Message(NoteOn, 10); // accent or whatever
	return NULL;
}

void
TNTrack::stopVoice(int note)
{
	output->Message(NoteOff, 0);
}

void 
TNTrack::Reset()
{
	vco.Message(SystemReset,0);
}

void 
TNTrack::goOff()
{ 
	stopVoice();
}

				
void				
TNTrack::SaveCustomSettings(BMessage& msg)
{
	msg.AddString("TNName","TN306");

	msg.AddFloat("TNCutoff",	 vcf.getCutoff());
	msg.AddFloat("TNResonance",  vcf.getResonance());
	msg.AddFloat("TNEnvMod",	 vcf.getEnvmod());
	msg.AddFloat("TNDecay",	     vcf.getDecay());		
	//dly
	msg.AddFloat("TNDelay",	 	 dly.getVol());
	msg.AddFloat("TNFeedback",	 dly.getFeedback());
	msg.AddFloat("TNDistortion", dly.getDistort());
	//seq
	msg.AddInt16("TNTune",		 getTune());
}


void
TNTrack::LoadCustomSettings(BMessage& msg)
{
	vcf.Cutoff	 ( msg.GetFloat("TNCutoff",		DEF_VCF_CUTOFF ));
	vcf.Resonance( msg.GetFloat("TNResonance",  DEF_VCF_RESO   ));
	vcf.Envmod	 ( msg.GetFloat("TNEnvMod",		DEF_VCF_ENVMOD ));
	vcf.Decay	 ( msg.GetFloat("TNDecay", 		DEF_VCF_DECAY  ));
	dly.Vol		 ( msg.GetFloat("TNDelay",		DEF_DLY_VOL    ));
	dly.Distort	 ( msg.GetFloat("TNDistortion", DEF_DLY_DIST   ));
	dly.Feedback ( msg.GetFloat("TNFeedback", 	DEF_DLY_FBK    ));
		Tune	 ( msg.GetInt16("TNTune", 		DEF_TUNE       ));
}
