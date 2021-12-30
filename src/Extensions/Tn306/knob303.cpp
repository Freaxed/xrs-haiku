#include "knob303.h"
#include "Utils.h"
#include "TNTrack.h"
#include <Window.h>
#include "TNKnob.h"

const 	rgb_color  	yellow 	={247,255,146,181};

#define	ACCENT		1983
#define	TIMEDELAY	1987

#define	POT_H		24

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
	
	
	tune  =  MakeKnob( "Tune",    &TB303Knobs::do_tune);	
	dist  =  MakeKnob( "Dist",    &TB303Knobs::do_dist);
	envmod = MakeKnob("EnvMod",   &TB303Knobs::do_envmod);
	decay  = MakeKnob("Decay",    &TB303Knobs::do_decay);	
	cutoff = MakeKnob("Cutoff",   &TB303Knobs::do_cutoff);	
	reso   = MakeKnob("reso",     &TB303Knobs::do_reso);	
	delay  = MakeKnob("Delay",    &TB303Knobs::do_delay);
	feedbk = MakeKnob("Feedback", &TB303Knobs::do_feedbk);
	tdelay = MakeKnob("TimeDelay", &TB303Knobs::do_timedelay);
	
	
	//manca tune
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
	
	vco=&track->vco; vcf=&track->vcf; vca=&track->vca; dly=&track->dly;
	
	if (Window()->Lock())
	{
		cutoff->SetValue(vcf->getCutoff());
		reso->SetValue(vcf->getResonance());
		envmod->SetValue(vcf->getEnvmod());
		decay->SetValue(vcf->getDecay());
	   	 delay->SetValue(dly->getVol());
		  dist->SetValue(dly->getDistort());
		feedbk->SetValue(dly->getFeedback());
		
		  tune->SetValue((float)(12 + fTrack->getTune())/24.0f);
		tdelay->SetValue(dly->getTimeDelay() * (1.0f/0.6f));
		Window()->Unlock();
	}
	
}


TB303Knobs::~TB303Knobs()
{
}

void TB303Knobs::do_tune(float t)      {  if (fTrack) fTrack->Tune((int)(t*24.0f - 12.0f)); }
void TB303Knobs::do_cutoff(float c)    {  vcf->Cutoff(c); }
void TB303Knobs::do_reso(float r)      {  vcf->Resonance(r); }
void TB303Knobs::do_envmod(float e)    {  vcf->Envmod(e); }
void TB303Knobs::do_decay(float d)     {  vcf->Decay(d); }
void TB303Knobs::do_delay(float d)     {  dly->Vol(d); }
void TB303Knobs::do_feedbk(float d)    {  dly->Feedback(d); }
void TB303Knobs::do_dist(float d)      {  dly->Distort(d); } //
void TB303Knobs::do_timedelay(float d) {  dly->TimeDelay(d * 0.6f); } //

void TB303Knobs::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
}

APot *
TB303Knobs::MakeController(BRect r,const char* title,const char* n2,int min,int max ,int cur,const rgb_color col,int32 ms)
{
	const BBitmap *x = LoadIcon("tnpot");
	APot	*p;
	p = new APot(BRect(r.left,r.top,r.left+POT_H,r.top+POT_H),"",new BMessage(ms),NULL,min,max,(BBitmap*)x,NULL);
	p->SetTarget(this);
	p->SetHighColor(col);
	p->distance = 12;
	p->SetValue(cur);
	//AddChild(p);
	return p;
}
typedef void (TB303Knobs::*update_value)(float);


TNKnob*
TB303Knobs::MakeKnob(const char* title, update_value func)
{
	return new TNKnob(title, func, this);
}
