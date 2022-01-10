#ifndef __KNOB303_H
#define __KNOB303_H

#include "vco303.h"
#include "vcf303.h"
#include "vca303.h"
#include "deverb.h"
#include <View.h>

class TNTrack;
class BStringView;
class TNKnob;


class TB303Knobs: public BView
{
public:
	
	typedef void (TB303Knobs::*update_value)(float);
	
				  TB303Knobs(BRect r);
				 ~TB303Knobs();
	void		  Reset(TNTrack*);
	
private:
		
	TNKnob *tune, *dist, *envmod, *decay, *cutoff, *reso, *delay, *feedbk, *tdelay;
	
	BStringView *label;
	
	TNTrack *fTrack;
	
	void do_tune(float t);
	void do_cutoff(float c);
	void do_reso(float r);
	void do_envmod(float e);
	void do_decay(float d);
	void do_delay(float d);
	void do_feedbk(float f);
	void do_dist(float d);
	void do_timedelay(float d);


	void AttachedToWindow();
	TNKnob* MakeKnob(const char* title, update_value func);
};

#endif

