#include <math.h>
#include "vcf303.h"

#define ENVINC 64
#define ENVBITS 6
#define ENVLEN 65536

VCF303::VCF303()
{
	envmod=0; cutoff=0;
	Cutoff(0);
	Resonance(0);
	Envmod(0);
	Decay(0);

	envpos=0;
	need_recalc=1;
	d1=d2=0;
}

VCF303::~VCF303()
{
}

void VCF303::Cutoff(float c)
{
	cutoff = c;
//	e1 = exp(6.109 + 1.5876*envmod + 2.1553*cutoff);
//	e0 = exp(5.613 - 0.8*envmod + 2.1553*cutoff);
// experimental:
//	e1 = exp(6.109 + 1.5876*envmod + 2.1553*cutoff - 3.02*(1.0-reso));
//	e0 = exp(5.613 - 0.8*envmod + 2.1553*cutoff - 2.86*(1.0-reso));
	e1 = exp(6.109 + 1.5876*envmod + 2.1553*cutoff - 1.2*(1.0-reso));
	e0 = exp(5.613 - 0.8*envmod + 2.1553*cutoff - 0.7696*(1.0-reso));
	e0*=M_PI/44100.0;
	e1*=M_PI/44100.0;
	e1 -= e0;
	need_recalc=1;
}

void VCF303::Resonance(float r)
{
	reso = r;
	Cutoff(cutoff);
}

void VCF303::Envmod(float e)	
{
	envmod=e;
	Cutoff(cutoff);
}

void VCF303::Decay(float d)
{
	linear_decay=d;
	d = 0.2 + (2.3*d);
	d*=44100;
	envdecay = pow(0.1, 1.0/d);
	need_recalc=1;
}

float VCF303::getDecay() { 
	return linear_decay;
}

// Only filters the left channel.  Pass single channels here and feed the
// output to panners/mixers for stereo effects.  Or instantiate two of them.
void VCF303::Process(const float *buf, int len, int channels)
{
	int i;
	long pos;

	if(need_recalc) recalc_env();

	float *a=abuf.GetBuf(), *b=bbuf.GetBuf();
	float c;
	float *out = sbuf.GetBuf(len/channels);
	
// This feedback low-pass filter comes from Musical Applications of
// Microprocessors by Hal Chamberlin.  a[] and b[] are calculated in
// recalc_env().
	for(i=0;i<len;i+=channels) {
		pos = envpos>>ENVBITS;
		c = 1 - a[pos] - b[pos];
		out[i] = a[pos]*d1 + b[pos]*d2 + c*buf[i];
		d2=d1; d1=out[i];
		if(envpos<(ENVLEN-1)) envpos++;
	}
	output->Process(out, len/channels, 1);
	// printf("loop! out[0]=%f\n",out[0]);
}

void VCF303::Message(SynthMessage msg, float data)
{
	switch(msg)
	{
		case NoteOn:
			envpos=0; // retrigger cutoff envelope
			break;
		case SystemReset:
			sbuf.Reset();
			break;
			
		default: break; // gcc is dumn
	}
	if(output) output->Message(msg, data);
}

void VCF303::recalc_env()
{
	float *a = abuf.GetBuf(ENVLEN>>ENVBITS);
	float *b = bbuf.GetBuf(ENVLEN>>ENVBITS);
	double decay16 = pow(envdecay, ENVINC);
	float whopping; // don't ask.
	float k;
	float r = exp(-1.20 + 3.455*reso);
	int i;

	c0=e1;
	for(i=0;i<(ENVLEN>>ENVBITS);i++) {
		whopping = e0+c0;
		k = exp(-whopping/r);
		c0*=decay16;

// This feedback low-pass filter comes from Musical Applications of
// Microprocessors by Hal Chamberlin.
		a[i] = 2.0*cos(2.0*whopping) * k;
		b[i] = -k*k;
	}
	need_recalc=0;
}



