/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TNTrackBoost.h"
#include "TNTrack.h"
#include "filemaker_msg.h"

#include <Message.h>
#include <File.h>
#include <Application.h>


TNTrackBoost::TNTrackBoost():TrackBoost()
{
	name << "Tn306";
	id=1;
	panel = new TNPanel(this);
}

Track*
TNTrackBoost::getTrack()
{
	return (Track*)(new TNTrack());
}


bool
TNTrackBoost::Load(Track* cur,int t,BFile* file,int32 va,int32 rt)
{
	BMessage *msg;
	switch(va)
	{
		case TRACK_SPECIFIC_DATA_START:
			msg = new BMessage();
			msg->Unflatten(file);
			LoadTrackSettings(cur,msg);
			return true;
			break;
		default:
			break;
	}	
		
	return false;
}
void
TNTrackBoost::LoadTrackSettings(Track* trk,BMessage* msg)
{
	TNTrack*	st = dynamic_cast<TNTrack*>(trk);
	
	if(!st)
		return;
	
	st->vcf.Cutoff(msg->FindFloat("TN_cutoff"));
	st->vcf.Resonance(msg->FindFloat("TN_resonance"));
	st->vcf.Envmod(msg->FindFloat("TN_envmod"));
	st->vcf.Decay(msg->FindFloat("TN_decay"));
	st->dly.Vol(msg->FindFloat("TN_delay"));
	st->dly.Distort(msg->FindFloat("TN_distortion"));
	st->dly.Feedback(msg->FindFloat("TN_feedback"));
	st->Tune(msg->FindInt16("TN_tune"));	
}


void
TNTrackBoost::SaveTrackSettings(Track* trk,BMessage* msg)
{
	
	TNTrack*	st = dynamic_cast<TNTrack*>(trk);
	
	if(!st)
		return;
	

	msg->AddString("TN_name","TN306");
	// vcf 
	msg->AddFloat("TN_cutoff",st->vcf.getCutoff());
	msg->AddFloat("TN_resonance",st->vcf.getResonance());
	msg->AddFloat("TN_envmod",st->vcf.getEnvmod());
	msg->AddFloat("TN_decay",st->vcf.getDecay());		
	//dly
	msg->AddFloat("TN_delay",st->dly.getVol());
	msg->AddFloat("TN_feedback",st->dly.getFeedback());
	msg->AddFloat("TN_distortion",st->dly.getDistort());
	
	//seq
	msg->AddInt16("TN_tune",st->getTune());
	
}
