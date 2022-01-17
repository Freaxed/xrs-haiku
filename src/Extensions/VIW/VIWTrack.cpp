/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "VIWTrack.h"
#include "Note.h"
#include "VSTInstrument.h"
#include "PlugWindow.h"
#include "XHost.h"
#include "viw_locale.h"
#include <AppKit.h>
#include <StorageKit.h>

#define	 FRAMES_NUM	1024


VIWTrack::VIWTrack():Track()
{
	stream_l=new float[ FRAMES_NUM];
	stream_r=new float[ FRAMES_NUM];
	stream_s[0]=stream_l;
	stream_s[1]=stream_r;

	

	curNote=new Note();
	viw=NULL;
	win=NULL;
	
	
	ResetName();
}	

void
VIWTrack::LoadVSTi(VSTPlugin* plugin)
{
	VSTInstrumentPlugin *m_plug;
	
	if(viw!=NULL)
	{
		XHost::Get()->SendMessage(X_UnregWindow,(void*)getWin());
		if(getWin()->Lock()) getWin()->Quit();
		
		m_plug=viw;
				
		XHost::Get()->SendMessage(X_LockSem,0);
			viw=NULL;		
		XHost::Get()->SendMessage(X_UnLockSem,0);
	
		delete m_plug;
	}
		
	if(plugin!=NULL)
	{
		m_plug=new VSTInstrumentPlugin(plugin);
		setWin(new PlugWindow(m_plug,true));
		
		XHost::Get()->SendMessage(X_RegWindow,(void*)getWin(),(void*)m_plug->EffectName());
		XHost::Get()->SendMessage(X_Switch,(void*)getWin());
		
		getWin()->SetTitle(getName());
		
		XHost::Get()->SendMessage(X_LockSem,0);
			viw=m_plug;		
		XHost::Get()->SendMessage(X_UnLockSem,0);
		Message(SystemReset,0);
	}
	
}
	
void VIWTrack::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case TempoChange: 
			// maybe it should be send?
			break;
		case SystemReset:
			//printf("VIW Reset\n");
			//stopVoice();
			if(viw) viw->StopAll(); 
		break;
		case	SystemStop:
			//printf("VIW Stop\n");
			stopVoice();
		break;
		
		default: break; // gcc is dumn
	}

}

void 
VIWTrack::Reset(){
	
	goOff();
	stopVoice();
}

VIWTrack::~VIWTrack(){

	delete stream_l;
	delete stream_r;
	LoadVSTi(NULL);
}

int
VIWTrack::getModel(){return 10;}


const char*
VIWTrack::getPreferredName() {
	return T_VIW_NAME;
} 

void
VIWTrack::Process(float** buffer,int32 samples_num,int spiaz,float factor)
{
	if(viw==NULL) return;
		
	memset((char*)stream_s[0], 0, samples_num*4);
	memset((char*)stream_s[1], 0, samples_num*4);

	//printf("------------ %f\n",stream_s[0][0]);
	viw->FilterFloat(stream_s,stream_s,samples_num,NULL);
	//printf("------------ %f\n", stream_s[0][0]);
	
		
	for (int L=0; L< samples_num; L++) {
			
		buffer[0][spiaz+L]+=stream_s[0][L]*Left() *factor;
		buffer[1][spiaz+L]+=stream_s[1][L]*Right()*factor;
	
	}
}


XRSVoice	
VIWTrack::newVoice(Note* n,int VoiceTag)
{ 
		
	if(viw==NULL) 
		return NULL;
	
	viw->SendNote(curNote->getNote(),1,false); //Note OFF
	
	curNote->setValue(n->getValue());
	curNote->setNote(n->getNote());
	
	viw->SendNote(curNote->getNote(),curNote->Gain());
	
	return NULL; 
}	
void 
VIWTrack::stopVoice(int note)
{
	if(viw==NULL) return;
	if(curNote==NULL) {
		viw->SendNote(60,100,false);
		return;
	}
	if(note<0 || note==curNote->getNote())	//Stop current note
		viw->SendNote(curNote->getNote(),curNote->Gain(),false);
	//else	viw->SendNote(note,100,false);
	
}


void VIWTrack::goOn()
{ 
	if(viw==NULL) 
		return; 
	//FIX viw->SetStatus(true); 
}

void 
VIWTrack::goOff()
{ 
	if(viw==NULL) 
		return;
	
	stopVoice();
	//FIX viw->SetStatus(false); 
}
						  
