#ifndef __DEVERB_H
#define __DEVERB_H

#include "synel.h"

//Potremmo convertirlo
//in un Deverb che funziona a float. :)

class Deverb: public Element
{


public:
	Deverb();
	virtual ~Deverb();

	void		Distort(float d);
	float		getDistort(); 
	
	void		Vol(float v) { amt=v; }
	float		getVol() { return amt;}
	
	void		Feedback(float f) { feedbk=f; }
	float		getFeedback() { return feedbk;}
	
	void		TimeDelay(float f) { time_factor=f; calc_timedelay(); }
	float		getTimeDelay() { return time_factor;}
	
	float *	getBuffer(int len);
	float * 	getBuffer();
	
	virtual void Process(const float *buf, int len, int channels);
	virtual void Message(SynthMessage msg, float data);
private:
	void calc_disttbl();
	SampleBuf delaybuf, outbuf;
	SampleBuf distbuf;
	float amt, feedbk, dist;
	// lowpass filter for determining wave magnitude
	float wavemag;

	int dlen, dpos;

	float		time_factor; //how many delay (respect a 4 beat?)
	float		tempo;
	void		calc_timedelay();
};

#endif

