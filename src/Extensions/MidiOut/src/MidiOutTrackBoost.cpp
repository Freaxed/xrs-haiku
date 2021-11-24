#include "MidiOutTrackBoost.h"
#include "Application.h"
#include "JTrack.h"
#include "MidiOutTrack.h"
#include "filemaker_msg.h"
#include "Message.h"
#include "File.h"
#include "GfxMsg.h"
#include	<Box.h>
#include	"MidiPanel.h"
// DINAMIC CAST!!!!!!!! //

#include "MidiProducer.h"
#include "XHost.h"
#include "midiout_locale.h"


MidiOutTrackBoost::MidiOutTrackBoost():TrackBoost()
{
	
	mider=(BMidiLocalProducer*)XHost::Get()->SendMessage(X_MidiProducer,NULL);

	name << T_MIDIOUT_NAME;
	id=2;
	
	mp=new MidiPanel();
	
}


PlugPanel*
MidiOutTrackBoost::getPanel(){

	return mp;
}

Track*
MidiOutTrackBoost::getTrack()
{
	return (Track*)(new MidiOutTrack(mider));
}


void
MidiOutTrackBoost::SaveTrackSettings(Track* trk,BMessage* data){

	MidiOutTrack*	st=(MidiOutTrack*)trk;
	data->AddInt16("output_channel",st->GetChannel());
	data->AddBool("send_on_play",st->GetSendOnPlay());	
	data->AddInt16("program_change",st->GetProgramChange());
	for(int i=0;i<3;i++)
	{
	 data->AddInt16("controller_number",st->GetCCNumber(i));
	 data->AddInt16("controller_value",st->GetCCValue(i));
	 printf("cc st->GetCCValue(i) %d\n",st->GetCCValue(i));
	}

}
void
MidiOutTrackBoost::LoadTrackSettings(Track* trk,BMessage* data){

	MidiOutTrack*	st=(MidiOutTrack*)trk;
	st->SetChannel(data->FindInt16("output_channel"));
	st->SetProgramChange(data->FindInt16("program_change"));
	st->SetSendOnPlay(data->FindBool("send_on_play"));
	int32 i=0;
	int16 num,val;
	
	while(data->FindInt16("controller_number",i,&num)==B_OK)
	{
		data->FindInt16("controller_value",i,&val);
		st->SetCCNumber(i,num);
		st->SetCCValue(i,val);
		i++;
	}
}
