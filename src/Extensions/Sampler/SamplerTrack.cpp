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

int vnum=0;

#include "pitchtable.h"
#include "SamplerVoice.h"

extern SamplerTrackBoost	*booster;


/*
1) plan: remove reverse.
2) remove pitch

*/

void SamplerTrack::Message(SynthMessage msg, float data)
{
	
	switch(msg) {
		case TempoChange: 
			samplesperbeat=(int32)data;
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
SamplerTrack::newVoice(Note* n,int VoiceTag)
{ 
	if(curSample == NULL) 
		return NULL;
	
	SamplerVoice* Voice = new SamplerVoice(n, curSample, mBuffers);
	Voice->tag 		= vnum;
	Voice->reverse	= reversed;
	//Voice->loop		= true;
		
	vnum++;
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
	while(Voice->GetNextFrames(&dest[0][x], &dest[1][x]) && x < sample_num)
	{
		dest[0][x] *= Left() * amp * curNote->Left();
		dest[1][x] *= Right()* amp * curNote->Right();
		x++;
	}

	return x;
//	if (Voice->is_done()) return 0;
//	
//	curNote = Voice->n;
//	
//	if(curSample     == NULL || 
//	   Voice->sample == NULL || 
//	   Voice->sample != curSample ) 
//	   return 0;
//	
//	uint32 x = 0;
//	do 
//	{
//		dest[0][x] = curSample->wave_data[0][Voice->get_position()] * Left() * amp * curNote->Left();
//		dest[1][x] = curSample->wave_data[1][Voice->get_position()] * Right()* amp * curNote->Right();
//		x++;
//		
//	} while(Voice->move_position() && x < sample_num);
//
//	return x;
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
	boost_enable=false;
	ResetName();
	//cit=false; //can we make a fast-fast decay
}

const char*
SamplerTrack::getPreferredName()
{
	if(curSample!=NULL) return curSample->name;
	else return T_SAMPLER_UNSET;
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


//__
