/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SamplerTrack.h"
#include "Pattern.h"
#include "Note.h"
#include "Sample.h"
#include <math.h>
#include "XHost.h"
#include "SamplerTrackBoost.h"
#include "sampler_locale.h"
#include "Log.h"
#include "pitchtable.h"
#include "SamplerVoice.h"

extern SamplerTrackBoost	*booster;


void SamplerTrack::Message(SynthMessage msg, float data)
{
	
	switch(msg) {
		case TempoChange: 
			samplesperbeat = data;
		break;
		case SystemStop:
			//kill dei looper!
		break;
		case SystemStart:
			//kill dei looper!
		break;
		default: break; // gcc is dumn
	}
	
}


XRSVoice
SamplerTrack::newVoice(Note* n, int VoiceTag)
{ 
	if(curSample == NULL) 
		return NULL;
		
	
	SamplerVoice* Voice = new SamplerVoice(n, curSample, mBuffers, res_enable ? numNotes : 0, samplesperbeat);
	Voice->reverse	= reversed;
	Voice->loop		= loop_enable;

	return (XRSVoice)Voice;
}

uint32	
SamplerTrack::ProcessVoice(XRSVoice v,float ** dest ,uint32 sample_num)
{
	SamplerVoice*	Voice = (SamplerVoice*)v;
	
	curNote = Voice->GetNote();
	
	if(curSample          == NULL || 
	   Voice->GetSample() == NULL || 
	   Voice->GetSample() != curSample ) 
	   return 0;

	uint32 x = 0;
	while(x < sample_num && Voice->GetNextFrames(&dest[0][x], &dest[1][x]))
	{
		dest[0][x] *= Left() * amp * curNote->Left();
		dest[1][x] *= Right()* amp * curNote->Right();
		x++;
	}
	return x;
}



void		
SamplerTrack::killVoice(XRSVoice v)
{
	free((SamplerVoice*)v);
}
   
SamplerTrack::SamplerTrack ():Track() 
{
	reversed=false;
	curSample=NULL;
	samplesperbeat=0;

	numNotes=16;
	res_enable=false;
	amp=1.;
	boost_enable = false;
	loop_enable  = false;
	ResetName();
	//cit=false; //can we make a fast-fast decay
}

const char*
SamplerTrack::getPreferredName()
{
	if (curSample) 
		return curSample->GetName();
	
	return T_SAMPLER_UNSET;
}
SamplerTrack::~SamplerTrack(){

 if(booster) booster->RemoveMe(this);

 }

Sample* SamplerTrack::getSample(){	return curSample; }

int		SamplerTrack::getModel(){return 0;}

bool	SamplerTrack::isReversed(){	return reversed;}
void	SamplerTrack::setReversed(bool val){	reversed=val; }
void	SamplerTrack::setSample(Sample* s){	curSample=s;}
void	SamplerTrack::setResampleEnable(bool val){	res_enable=val; }
bool	SamplerTrack::isResampleEnable() { return res_enable; }

int		SamplerTrack::getResample(){ return (int)(numNotes);}
void	SamplerTrack::setResample(int val){	numNotes=(float)val; }


void				
SamplerTrack::SaveCustomSettings(BMessage& track)
{
	if(getSample()) 
		track.AddString("SamplePath", getSample()->GetFullPath());
		
	track.AddBool("ResampleEnabled",isResampleEnable());	
    track.AddInt16("Resample", getResample());
	
	track.AddFloat("Boost", amp);
	
	track.AddBool("BoostEnabled",isBoostEnable());
	track.AddBool("Reversed",isReversed());
	
	track.AddBool("Loop", IsLoopEnable());
}

void				
SamplerTrack::LoadCustomSettings(BMessage& track)
{
	assert(booster);
	BString path;
	if (track.FindString("SamplePath", &path) == B_OK)
	{
		Sample *s = booster->FindSample(BPath(path.String()).Leaf());
		if(s)
		{
			booster->_secureSetSample(this, s);
			assert(s == getSample());
			setName(getSample()->GetName());
		}
	}


	setResampleEnable(track.GetBool("ResampleEnabled", false));
	setResample(track.GetInt16("Resample", 16));
	amp = track.GetFloat("Boost", 1.0f);
	setBoostEnable(track.GetBool("BoostEnabled", false));
	setReversed(track.GetBool("Reversed", false));
	SetLoopEnable(track.GetBool("Loop", false));
}
	

//__
