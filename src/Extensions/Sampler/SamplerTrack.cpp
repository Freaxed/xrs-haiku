/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SamplerTrack.h"
#include "Pattern.h"
#include "Note.h"
#include "Sample.h"
#include "samplefiller.h"
#include <math.h>
#include "XHost.h"
#include "SamplerTrackBoost.h"
#include "sampler_locale.h"

#define THUMB	50;
int vnum=0;
#include "pitchtable.h"
#include "SamplerVoice.h"

//#define DECAY_LEN 10

extern SamplerTrackBoost	*booster;




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
SamplerTrack::newVoice(Note* n,int VoiceTag){ 
	
	
	if(curSample==NULL) return NULL;
	
	SamplerVoice* Voice;
  
    Voice = new SamplerVoice;
    Voice->n = n;
	Voice->position = 0;
	Voice->tag=vnum; //VoiceTag;
	Voice->sample=curSample;
	Voice->converter=NULL;
	Voice->refiller=NULL;
	Voice->reverse=isReversed();
	float coef=1.;
	
	if(res_enable){
		coef	= (float)( (float)(curSample->frames/curSample->channels) /   (float)(samplesperbeat*(numNotes/4)) );
		coef	*=curSample->freq_divisor;
		//printf("[%d] coef %f\n",curSample->channels,coef);
		//coef = 0.537451;
	}
	else
	if(n->getNote()!=60){
	
		coef=pitchtable[n->getNote()]/pitchtable[60];
	}
	// debug
	if(coef==0) coef=1.;
	
	
	coef = coef / curSample->freq_divisor;
		
	if(coef!=1.) {	
		
		Voice->refiller = new SampleRefiller(Voice->sample);
		Resampler *arts = new Resampler(Voice->refiller,(const void*)Voice);
		arts->setStep(coef);
		arts->setChannels(Voice->sample->channels);
		arts->setBits(16);
		
		
		Voice->converter=arts;
	}
					
				
	
		
	vnum++;
	return (XRSVoice)Voice;
}
		
int32	
SamplerTrack::ProcessVoice(XRSVoice v,float ** dest ,int32 sample_num){

	SamplerVoice*	Voice=(SamplerVoice*)v;
	int16 			data[2];
		
	/*
	 annotazioni:
	 lengh = quanto in questo process riempire di buffer (o tutto o di un tot da Voice->position)
	  	    se è meno del previsto allora la voce verrà successivmente eliminata.
	*/
	curNote=Voice->n;
	
	if(curSample==NULL || Voice->sample==NULL || Voice->sample!=curSample ) return 0;
	
	int32 fullframes= curSample->frames / curSample->channels;
	int32 length= MIN(fullframes-Voice->position,(uint32)sample_num);
	int32 position=0;
	
	if(Voice->converter==NULL){

		for(int32 l=0;l<length;l++)
		{
			if(isReversed()) //reverse
			  position=fullframes-Voice->position-l-1; //MA QUI E' GIUSTO? O VA IL -uno ???
			else
			  position=Voice->position+l;
					
			data[0]=curSample->wave_data[(position)*curSample->channels];
			
			if(curSample->channels==2)
				//ANCHE QUI MI SA CHE SE è REVERSE è SBAGLIATO..!!  +1 DI COSA? 
				data[1]=curSample->wave_data[(position)*curSample->channels+1];
			else
				data[1]=data[0];	
							
			dest[0][l] = (float)data[0]/32767.0f * Left() * amp * curNote->Left();
			dest[1][l] = (float)data[1]/32767.0f * Right()* amp * curNote->Right();
		}
		
		Voice->position +=length;
		
		if(Voice->position > fullframes)	length=0;
	 	
	}
	
	else
	
	{
		int retz = Voice->converter->run(dest[0],dest[1],length);
		if(retz!=length)  retz-=1; //pixel bug :) 
		
		for(int32 l=0;l<length;l++)
		{
			dest[0][l] *= Left() * amp * curNote->Left();
			dest[1][l] *= Right()* amp * curNote->Right();
		}
		length = retz;
	}
		
	return length;
	
}
	
void		
SamplerTrack::killVoice(XRSVoice v){

		SamplerVoice*	Voice=(SamplerVoice*)v;
		
		if(Voice->converter!=NULL) 
		{
			delete Voice->refiller;		
			delete Voice->converter;
		}
		free(Voice);
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
