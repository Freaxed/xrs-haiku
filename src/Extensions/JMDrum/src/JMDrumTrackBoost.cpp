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
	return (Track*)(new JMDrumTrack());
}

PlugPanel*
JMDrumTrackBoost::getPanel()
{
	return panel;
}





//--
