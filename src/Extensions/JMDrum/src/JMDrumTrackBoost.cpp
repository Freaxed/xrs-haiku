#include "JMDrumTrackBoost.h"

#include <Application.h>
#include <Message.h>
#include <File.h>
#include <Path.h>

#include "JMDrumTrack.h"
#include "filemaker_msg.h"
#include "GfxMsg.h"
#include "TrackManager.h"
#include "PlugPanel.h"
#include "JMDrumPanel.h"
#define	MY_ID	8;
#include "MachineInterface.h"



JMDrumPanel* panel;
JMDrumTrackBoost* jm_booster=NULL;

extern CMachineInfo const * GetInfo();
const CMachineInfo* info;
extern void ParameterTweak(int par, int val,int* Vals,DrumPars *globalpar);

JMDrumTrackBoost::JMDrumTrackBoost():TrackBoost()
{
	jm_booster=this;
	info=(CMachineInfo* )GetInfo();
	name << "JMDrum Synth";
	
	id=MY_ID;
	panel=new JMDrumPanel();

}

Track*
JMDrumTrackBoost::getTrack()
{
	JMDrumTrack *trk=new JMDrumTrack();
	return (Track*)(trk);
}

PlugPanel*
JMDrumTrackBoost::getPanel()
{
	return panel;
}


void
JMDrumTrackBoost::LoadTrackSettings(Track* trk,BMessage* data)
{
	JMDrumTrack	*t=(JMDrumTrack*)trk;
	int16 v;
	for(int i=0;i<16;i++)
		{
		 data->FindInt16("Vals",i,&v);
		 ParameterTweak(i,v,t->Vals,&t->values);
		}
}

void			
JMDrumTrackBoost::SaveTrackSettings(Track* trk,BMessage* data)
{
	JMDrumTrack	*t=(JMDrumTrack*)trk;
	
	for(int i=0;i<16;i++)
		data->AddInt16("Vals",t->Vals[i]);
}

//--
