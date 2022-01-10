#include "knob303.h"
#include "Utils.h"
#include "TNTrack.h"
#include "TNKnob.h"

#include <Window.h>
#include <stdio.h>
#include <Application.h>
#include <Resources.h>
#include <TranslationUtils.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>

TB303Knobs::TB303Knobs(BRect r) : BView(r, "TN306", B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS)
{
	BGridLayout* grid = BLayoutBuilder::Grid<>();
	SetLayout(grid);
	
	grid->SetSpacing(0, 0);	
	
	tune  =  MakeKnob("Tune",      &TB303Knobs::do_tune);	
	dist  =  MakeKnob("Dist",      &TB303Knobs::do_dist);
	envmod = MakeKnob("EnvMod",    &TB303Knobs::do_envmod);
	decay  = MakeKnob("Decay",     &TB303Knobs::do_decay);	
	cutoff = MakeKnob("Cutoff",    &TB303Knobs::do_cutoff);	
	reso   = MakeKnob("reso",      &TB303Knobs::do_reso);	
	delay  = MakeKnob("Delay",     &TB303Knobs::do_delay);
	feedbk = MakeKnob("Feedback",  &TB303Knobs::do_feedbk);
	tdelay = MakeKnob("TimeDelay", &TB303Knobs::do_timedelay);	
	
	grid->AddView(tune, 	0, 0);
	grid->AddView(dist, 	1, 0);	
	grid->AddView(envmod, 	0, 1);	
	grid->AddView(decay, 	1, 1);
	grid->AddView(cutoff, 	2, 1);	
	grid->AddView(reso, 	0, 2);
	grid->AddView(delay, 	1, 2);
	grid->AddView(feedbk, 	2, 2);
	grid->AddView(tdelay, 	3, 2);	
	
	
	fTrack = NULL;
		
	tune->SetValue(0.5f); //0->tuned	
}

void
TB303Knobs::Reset(TNTrack *track)
{
	fTrack = track; 
	
	if(!fTrack)
		return;
	
	if (Window()->Lock()) //?
	{
	   cutoff->SetValue(fTrack->vcf.getCutoff());
		 reso->SetValue(fTrack->vcf.getResonance());
	   envmod->SetValue(fTrack->vcf.getEnvmod());
		decay->SetValue(fTrack->vcf.getDecay());
	   	delay->SetValue(fTrack->dly.getVol());
		 dist->SetValue(fTrack->dly.getDistort());
	   feedbk->SetValue(fTrack->dly.getFeedback());
    	 tune->SetValue((float)(12 + fTrack->getTune())/24.0f);
	   tdelay->SetValue(fTrack->dly.getTimeDelay() * (1.0f/0.6f));
	  
	  Window()->Unlock();
	}
	
}


TB303Knobs::~TB303Knobs()
{
}

void TB303Knobs::do_tune(float t)      {  if (fTrack) fTrack->Tune((int)(t*24.0f - 12.0f)); }
void TB303Knobs::do_cutoff(float c)    {  if (fTrack) fTrack->vcf.Cutoff(c); }
void TB303Knobs::do_reso(float r)      {  if (fTrack) fTrack->vcf.Resonance(r); }
void TB303Knobs::do_envmod(float e)    {  if (fTrack) fTrack->vcf.Envmod(e); }
void TB303Knobs::do_decay(float d)     {  if (fTrack) fTrack->vcf.Decay(d); }
void TB303Knobs::do_delay(float d)     {  if (fTrack) fTrack->dly.Vol(d); }
void TB303Knobs::do_feedbk(float d)    {  if (fTrack) fTrack->dly.Feedback(d); }
void TB303Knobs::do_dist(float d)      {  if (fTrack) fTrack->dly.Distort(d); } //
void TB303Knobs::do_timedelay(float d) {  if (fTrack) fTrack->dly.TimeDelay(d * 0.6f); } //

void TB303Knobs::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
}



TNKnob*
TB303Knobs::MakeKnob(const char* title, update_value func)
{
	return new TNKnob(title, func, this);
}
