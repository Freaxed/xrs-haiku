/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TNTrack_H_
#define _TNTrack_H_

#include "Track.h"
#include "synel.h"
#include "vco303.h"
#include "vcf303.h"
#include "vca303.h"
#include "deverb.h"

class  TNTrackBoost;

class TNTrack : public Track
{
	public:
	
				  	TNTrack();
	
	void			Reset();
	void			Process(float**,int32,int,float factor=1.0);
	XRSVoice		newVoice(Note* n,int VoiceTag);	
	void			stopVoice(int note=-1);	
	int				getModel();
	void			goOff();
	void 			Message(SynthMessage msg, float data);
	bool			SupportPanNote(){ return true; };
	bool			SupportSustainNote(){ return true;};
	
	void 			Tune(int halfsteps) { tuneoffset=halfsteps; }
	int 			getTune() { return tuneoffset;}
	
	TRACK_GEN_TYPE	getProcessorType() { return TT_VOICE_PROCESS; }

	VCO303		vco;
	VCF303 		vcf;
	VCA303 		vca;
	Deverb 		dly;
		
	float		winx,winy,Ysize;
	bool 		big,hidden;
		
	protected:
	
		virtual 	const char*	getPreferredName(){ return "Tn306";}
	
	private:
			
			int 				tuneoffset;
			Element*			output;
			int32				note_length;
			int32				current_note_length;
};

#endif
