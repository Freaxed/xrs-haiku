#ifndef __KNOB303_H
#define __KNOB303_H

#include "APot.h"
//#include "seq.h"
#include "vco303.h"
#include "vcf303.h"
#include "vca303.h"
#include "deverb.h"
#include <View.h>
#include <TextControl.h>

class TNTrack;
class BStringView;
class TNKnob;




class TB303Knobs: public BView
{
public:
	typedef void (TB303Knobs::*update_value)(float);
	
private:
		
	//APot  *accent;
	TNKnob *tune, *dist, *envmod, *decay, *cutoff, *reso, *delay, *feedbk, *tdelay;
	
	BStringView *label;
	
	TNTrack *fTrack;
	VCO303 *vco;
	VCF303 *vcf;
	VCA303 *vca;
	Deverb *dly;
	BTextControl*	name_label;

public:
			TB303Knobs(BRect r);
			
	void		Reset(TNTrack*);
	
	 ~TB303Knobs();


private:	
	void do_tune(float t);
	void do_cutoff(float c);
	void do_reso(float r);
	void do_envmod(float e);
	void do_decay(float d);
	void do_delay(float d);
	void do_feedbk(float f);
	void do_dist(float d);
	void do_timedelay(float d);

protected:
	void AttachedToWindow();
	APot * MakeController(BRect r,const char* n1,const char* n2,int min,int max ,int cur,const rgb_color col,int32);
	TNKnob* MakeKnob(const char* title, update_value func);
};

#endif

