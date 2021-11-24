#include "MidiOutTrack.h"
#include "stdio.h"
#include "Note.h"
#include <math.h>
#include "stdio.h"
#include "Pattern.h"
#include "Note.h"
#include "MidiProducer.h"

MidiOutTrack::MidiOutTrack(BMidiLocalProducer *m):Track()
{
	mider=m;
	curNote=new Note();
	channel=0;
	latency=0;
	program_change=0;
	for(int i=0;i<MAX_CC;i++)
		my_trk_contr[i].value=my_trk_contr[i].number=0;
		
	ResetName();	
	sendOnPlay=false;
}

MidiOutTrack::~MidiOutTrack(){}

int MidiOutTrack::getModel(){return 2;}

const char*
MidiOutTrack::getPreferredName()
{
	return T_MIDIOUT_NAME;
}

void MidiOutTrack::Message(SynthMessage msg, float data)
{
	//int n;
	
	switch(msg) {
		case TempoChange: 
			// :) DA INVIARE!
			break;
		case SystemReset:
			//if(!sendOnPlay) return;
			//mider->SprayProgramChange(channel,0);
		break;
		case	SystemStop:
			//mider->SprayControlChange(channel,123,0,latency);
			stopVoice();
		break;
		case	SystemStart:
			stopVoice();
			if(!sendOnPlay) return;
			SetProgramChange(program_change);
				for(int i=0;i<MAX_CC;i++)
				{
					SetCCNumber(i,my_trk_contr[i].number);
					SetCCValue(i,my_trk_contr[i].value);
					
				}
		break;
		default: break; // gcc is dumn
	}
	
}



XRSVoice
MidiOutTrack::newVoice(Note* n,int VoiceTag)
{
	if(curNote!=NULL)
		mider->SprayNoteOff(channel,curNote->getNote(),curNote->GetValue(0),latency);
	
	curNote->setNote(n->getNote());
	curNote->SetValue(n->GetValue(0));
	
	mider->SprayNoteOn(channel,curNote->getNote(),curNote->GetValue(0),latency);
	return NULL;
}

void
MidiOutTrack::stopVoice(int note)
{
	if(curNote==NULL) return;
	if(note<0)
		mider->SprayNoteOff(channel,curNote->getNote(),curNote->getVolume()*100,latency);
	else
		mider->SprayNoteOff(channel,note,0,latency);
}
void
MidiOutTrack::SetProgramChange(int pg)
{
	program_change=pg;
	mider->SprayProgramChange(channel,program_change,latency);
}
void MidiOutTrack::SetChannel(int n){ stopVoice();	channel=n; }


void MidiOutTrack::SetCCValue(int n,int value){
	my_trk_contr[n].value=value;
	mider->SprayControlChange(channel,my_trk_contr[n].number,value,latency);
}
void MidiOutTrack::SetCCNumber(int n,int value){
	my_trk_contr[n].number=value;
}

void MidiOutTrack::setVolume(float val)
{
	uchar vol= (uchar)(val*127.0);
	mider->SprayControlChange(channel,7,vol,latency);
	Track::setVolume(val);
}
void MidiOutTrack::setPan(float val)
{
	int vol= (int)((val+1.0)*63.0);
	mider->SprayControlChange(channel,10,vol,latency);
	Track::setPan(val);
}
void MidiOutTrack::Reset(){}
void MidiOutTrack::goOn(){ }
void MidiOutTrack::goOff(){ }
