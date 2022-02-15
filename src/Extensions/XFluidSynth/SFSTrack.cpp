#include 	"SFSTrack.h"
#include 	"Note.h"
#include	"XHost.h"

#include "SFSTrackBoost.h"

#include <AppKit.h>
#include <StorageKit.h>


#define	 FRAMES_NUM	1024


extern	fluid_settings_t* settings;
extern 	SFSTrackBoost*	  fluid_booster;
extern	XFSynth			*ptheSynth;
		
SFSTrack::SFSTrack():Track()
{

		
	stream_l=new float[ FRAMES_NUM];
	stream_r=new float[ FRAMES_NUM];

	channel=0;
	curNote=new Note();
	hasdata=false;
	
	reverb_send=chorus_send=0;
	ResetName();
}


	
void SFSTrack::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case TempoChange: 
			
		break;
		case SystemReset:
		break;
		
		case	SystemStop:
			if(curNote!=NULL)
				ptheSynth->NoteOff(channel, curNote->getNote());
		break;
		
		default: break; // gcc is dumn
	}

}
void
SFSTrack::SetChannel(int ch)
{
	
	if(curNote!=NULL)
				ptheSynth->NoteOff(channel, curNote->getNote());
	channel=ch;
}

void 
SFSTrack::Reset(){}

SFSTrack::~SFSTrack(){
	if(fluid_booster) fluid_booster->RemoveMe(this);
}

int
SFSTrack::getModel(){return 7;}


const char*
SFSTrack::getPreferredName() {
	return "XFluidSynth";
} 

void SFSTrack::Process(float** buffer,int32 samples_num,int spiaz,float factor)
{
	if(!ptheSynth->IsValid()) return;
	memset((char*)stream_l, 0, samples_num*4);
	memset((char*)stream_r, 0, samples_num*4);
	
	ptheSynth->WriteFloat(samples_num,stream_l,stream_r);
	
	for (int L=0; L< samples_num; L++) {
			
		buffer[0][spiaz+L] += stream_l[L]*Left();
		buffer[1][spiaz+L] += stream_r[L]*Right();
	
	}
}


XRSVoice	
SFSTrack::newVoice(Note* n,int VoiceTag){

	if(!ptheSynth->IsValid()) return NULL;
	if(curNote!=NULL)
		ptheSynth->NoteOff(channel, curNote->getNote());
		
	curNote->setNote(n->getNote());
	curNote->SetGain(n->Gain());
	
	ptheSynth->NoteOn(channel, curNote->getNote(), (int)(curNote->Gain() * 127.0f));
	
	return NULL; 
		
}	
void SFSTrack::stopVoice(int note) {
	
	if(!ptheSynth->IsValid()) return;
		ptheSynth->NoteOff(channel, note);

}

void SFSTrack::goOn(){}
void SFSTrack::goOff(){

	if(!ptheSynth->IsValid()) return;
	if(curNote!=NULL)
			ptheSynth->NoteOff(channel, curNote->getNote());
}

void
SFSTrack::RouteLineChanged(int line)
{
	// now dirty things.. :) /* you are not reading the following code..*/
	fluid_booster->SetLine(line,this);
}


void
SFSTrack::SaveCustomSettings(BMessage& msg)
{
	msg.AddInt16("Channel",GetChannel());
	
	ptheSynth->SaveChannelSettings(&msg, GetChannel());
	
	msg.AddInt16("Reverb", GetReverbSend());
	msg.AddInt16("Chorus", GetChorusSend());
			
}

void
SFSTrack::LoadCustomSettings(BMessage& msg)
{

	SetChannel(msg.FindInt16("channel"));
	
	ptheSynth->LoadChannelSettings(&msg, GetChannel());
	
	SetReverbSend(msg.FindInt16("Reverb"));
	SetChorusSend(msg.FindInt16("Chorus"));
}
