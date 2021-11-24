#include "knob303.h"
#include "Utils.h"
#include "TNTrack.h"
#include <Window.h>

const 	rgb_color  	yellow 	={247,255,146,181};

#define	TUNE		1978
#define CUTOFF	1979
#define	RESO		1980
#define	ENVMOD	1981
#define	DECAY	1982
#define	ACCENT	1983
#define	DELAY		1984
#define	FEEDBK		1985
#define	DIST		1986
#define	TIMEDELAY	1987

#define	POT_H		24

#include <stdio.h>
#include <Application.h>
#include <Resources.h>
#include <TranslationUtils.h>


TB303Knobs::TB303Knobs(BRect r) : BView(r, "TN306", B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS)
{
	
	
	tune  = MakeController(BRect(21 ,38,0,0),"tune","tune",-12,12,-12,yellow,TUNE);
	dist  = MakeController(BRect(61 ,38,0,0),"dist","dist",0,127,64,yellow,DIST);
	
	cutoff= MakeController(BRect(101,90,0,0),"cutoff","cutoff",0,127,0,yellow,CUTOFF);
	reso  = MakeController(BRect(141,90,0,0),"reso","reso",0,127,0,yellow,RESO);
	envmod= MakeController(BRect(21 ,90,0,0),"EnvMod","EnvMod",0,127,0,yellow,ENVMOD);
	decay = MakeController(BRect(61 ,90,0,0),"decay","Decay",0,127.,0,yellow,DECAY);
	
	delay = MakeController(BRect(21 ,141,0,0),"delay","delay",0,127,0,yellow,DELAY);
	feedbk= MakeController(BRect(61 ,141,0,0),"feedback","feedback",0,127,0,yellow,FEEDBK);
	tdelay= MakeController(BRect(101,141,0,0),"TimeDelay","TimeDelay",1,60,0,yellow,TIMEDELAY);
	
	
	
	
	fTrack = NULL;
		
	tune->SetValue(0);
	
	
	SetViewColor(0,0,156);
	SetLowColor(0,0,156);
	SetHighColor(255,255,255);
	
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
		cutoff->SetValue(vcf->getCutoff()*128);
		reso->SetValue(vcf->getResonance()*128);
		envmod->SetValue(vcf->getEnvmod()*128);
		
		decay->SetValue(vcf->getDecay()*128.);
		
		tdelay->SetValue(dly->getTimeDelay()*10);
		
		delay->SetValue(dly->getVol()*128);
		dist->SetValue(dly->getDistort()*128);
		feedbk->SetValue(dly->getFeedback()*128);
		
		tune->SetValue(fTrack->getTune());

		Window()->Unlock();
	}
	
}


TB303Knobs::~TB303Knobs()
{
}

void TB303Knobs::do_tune(float t) { fTrack->Tune((int)t); }
void TB303Knobs::do_cutoff(float c) {  vcf->Cutoff(c); }
void TB303Knobs::do_reso(float r) {  vcf->Resonance(r); }
void TB303Knobs::do_envmod(float e) {  vcf->Envmod(e); }
void TB303Knobs::do_decay(float d) {  vcf->Decay(d); }
void TB303Knobs::do_delay(float d) {  dly->Vol(d); }
void TB303Knobs::do_feedbk(float d) {  dly->Feedback(d); }
void TB303Knobs::do_dist(float d) {  dly->Distort(d); } //
void TB303Knobs::do_timedelay(float d) {  dly->TimeDelay(d); } //

void
TB303Knobs::MessageReceived(BMessage* message)
{
	if(fTrack == NULL) 
		return;
	
	switch(message->what)
	{
		
		case TUNE:
			
			do_tune((float)tune->Value());
			break;
		case CUTOFF:
			
			do_cutoff((float)cutoff->Value()/128.);
		case RESO:
			
			do_reso((float)reso->Value()/128.);
			break;
		case ENVMOD:
			
			do_envmod((float)envmod->Value()/128.);
			break;
		case DECAY:
			
			do_decay((float)decay->Value()/128.);
			break;
		case TIMEDELAY:
			
			do_timedelay((float)tdelay->Value()/10.);
			break;
		case DELAY:
			do_delay((float)delay->Value()/128.);
			break;
		case FEEDBK:
			
			do_feedbk((float)feedbk->Value()/128.);
			break;
		case DIST:
			
			do_dist((float)dist->Value()/128.);
			break;
	 	
	 	default:
			BView::MessageReceived(message);
	}
}
void TB303Knobs::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
		
	tune->SetTarget(this); 
	cutoff->SetTarget(this);
	reso->SetTarget(this);
	envmod->SetTarget(this);
	decay->SetTarget(this);
	tdelay->SetTarget(this);
	delay->SetTarget(this); 
	feedbk->SetTarget(this);
	dist->SetTarget(this);
}

APot *
TB303Knobs::MakeController(BRect r,const char* n1,const char* n2,int min,int max ,int cur,const rgb_color col,int32 ms)
{
	const BBitmap *x = LoadIcon("tnpot");
	APot	*p;
	p = new APot(BRect(r.left,r.top,r.left+POT_H,r.top+POT_H),"",new BMessage(ms),NULL,min,max,(BBitmap*)x,NULL);
	p->SetTarget(this);
	p->SetHighColor(col);
	p->distance = 12;
	p->SetValue(cur);
	AddChild(p);
	return p;
}
