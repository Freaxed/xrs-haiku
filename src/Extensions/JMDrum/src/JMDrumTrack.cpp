#include 	"JMDrumTrack.h"
#include 	"Note.h"
#include	"XHost.h"

#include <AppKit.h>
#include <StorageKit.h>

#include "MachineInterface.h"
//#include <iostream.h>

#define	 FRAMES_NUM	1024


extern void ParameterTweak(int par, int val,int* Vals,DrumPars *globalpar);
extern const CMachineInfo* info;



JMDrumTrack::JMDrumTrack():Track()
{

		Vals=new int[16];
		
		//cleanup!
		for(int i=0;i<16;i++) Vals[i]=0;
		memset(&values,0,sizeof(struct DP));
			
		/*	printf(" ** Psycle Machine Information **\n");
			
			if(info->Flags & GENERATOR)
			 printf("** GENERATOR:\n");
				else
			 printf("** EFFECT:\n");
				
			printf(" Version: %d\n",info->Version);
			printf(" numParameters %d\n",info->numParameters);
			printf(" Name: %s\n",info->Name);
			printf(" ShortName: %s\n",info->ShortName);
			printf(" Author: %s\n",info->Author);
			printf(" Command: %s\n",info->Command);
			memset(&values,0,sizeof(values));
		*/	
		
		for (int i = 0; i<info->numParameters; i++) {
			ParameterTweak(i, info->Parameters[i]->DefValue,Vals,&values);
		}
		
		ResetName();
		cit=false;

	/*
	float StartSpeed;	// Speed at which the Oscillator will run.
	float IncSpeed; 	// Speed at which the Oscillator speed is increased.
	int DecMode;		// Function that will be applied to the speed increase.
	float DecLength;	// Number of samples that DecFunction is applied.
	*/
	 
	/*
	float SinVol;
	int AttackPos;
	int DecayPos;
	float AttackInc;
	float DecayDec;
	float SustainDec;
	int SLength;		// Total number of samples to play.

	int ThumpLength;	// Number of samples to play the thump.
	float ThumpVol;
	float ThumpDec;
	int ThumpFreq;

	float OutVol;			// Out Volume
	int Mix;
	int sinmix;
	int samplerate;
	*/
	
	//printf("%f %f %d %f\n",values.StartSpeed,values.IncSpeed,values.DecMode,values.DecLength);
	//printf("%d >%f %f >%f %d\n",values.sinmix,values.AttackInc,values.DecayDec,values.SustainDec,values.SLength);
				
}
	
	
void JMDrumTrack::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case TempoChange: 
			
			break;
		case SystemReset:
			
		break;
		
		case	SystemStop:
		
		break;
		
		default: break; // gcc is dumn
	}

}

uint32			
JMDrumTrack::ProcessVoice(XRSVoice voice, float** buffer, uint32 samples_num)
{
	Drum*	drum = (Drum*) voice;
	Note *n=(Note*)drum->cookie; //quick hack
	uint32 i = 0;
	for(i=0;i<samples_num;i++)
	{
		if(drum->AmpEnvStage)
		{
			float sl=(drum->GetSample()/32767.0f); //? are we sure?
			buffer[0][i] = sl*Left()   * n->Left();
	 		buffer[1][i] = sl*Right()  * n->Right();
		}
		else
		 break;				
	}
	return i;
}

XRSVoice	
JMDrumTrack::newVoice(Note* n,int VoiceTag)
{	
	Drum *d=new Drum();
	
	
	//noteon
	values.OutVol=(n->Gain());
	
	if (Vals[8] == 0)	// If Mode 1.x
	
		d->NoteOn(48,&values); // C-4 always
	else
		d->NoteOn(n->getNote(),&values);
		
	
	d->cookie=(void*)n;
	
	//end noteon
	
	return (XRSVoice)d;
}

void			
JMDrumTrack::killVoice(XRSVoice voice)
{
	Drum*	drum=(Drum*)voice;
	delete drum;
}


JMDrumTrack::~JMDrumTrack(){ delete Vals;}

int
JMDrumTrack::getModel(){return 8;}


const char*
JMDrumTrack::getPreferredName() {
	return "JMDrum";
} 


void JMDrumTrack::stopVoice(int note){}
void JMDrumTrack::goOn(){}
void JMDrumTrack::goOff(){}

void
JMDrumTrack::SaveCustomSettings(BMessage& msg)
{
	for(int i=0;i<16;i++)
		msg.AddInt16("Vals", Vals[i]);
}

void
JMDrumTrack::LoadCustomSettings(BMessage& msg)
{
	int16 v;
	for(int i=0;i<16;i++)
	{
		 msg.FindInt16("Vals",i,&v);
		 ParameterTweak(i,v, Vals,&values);
	}
}
//--
