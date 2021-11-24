#include "deverb.h"
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>

Deverb::Deverb()
{
	Vol(0);
	Feedback(0);
	Distort(0.5);
	
	dlen=4096; 
	dpos=0;
	wavemag=0;
	time_factor=0.25;
	tempo=0;
		
	float *b = delaybuf.GetBuf(dlen);
	memset(b, 0, 4096*sizeof(float));
	calc_disttbl();
}

Deverb::~Deverb()
{
}

void Deverb::Process(const float *buf, int len, int channels)
{
	
//	if(!enable) {
//		output->Process(buf, len, channels);
//		return;
//	}
	float *dbuf = delaybuf.GetBuf(dlen);
	float *out = outbuf.GetBuf(len);
	float *dtbl = distbuf.GetBuf();
	float xi;
	int i, dind = dpos-1;
	if(dind<0) dind+=dlen;

	wavemag = 8000+(1000/(dist+0.1));
	
		
	for(i=0;i<len;i++) {
		xi = buf[i];
		if(xi>32767) xi=32767;
		else if(xi<-32767) xi=-32767;
		xi += dist*dtbl[(long) (32768.0+xi)];
		if(xi > wavemag) xi=wavemag;
		if(xi < -wavemag) xi=-wavemag;

		//dist.

		out[i] = dbuf[dpos] + xi;
		dbuf[dind++]=amt*xi + feedbk*dbuf[dpos++];
		if(dind>=dlen) dind-=dlen;
		if(dpos>=dlen) dpos-=dlen;
	}
	
	if(output)
	output->Process(out, len, channels);
}

void Deverb::Message(SynthMessage msg, float data)
{
	switch(msg)
	{
		case TempoChange:
			tempo=data;
			calc_timedelay();
			break;
		case SystemReset:
			
			outbuf.Reset();
			delaybuf.Reset();
					
			break;
			
		default: break; // i'm sick of that warning.
	}
	
	if(output) output->Message(msg, data);
}

void
Deverb::calc_timedelay()
{
	if(tempo==0) return;
	
	float newlen;
	newlen = tempo*time_factor; // ?
	if((int)newlen > dlen) {
		float *b = delaybuf.GetBuf((int)newlen);
		memset(b+dlen, 0, (((int)newlen)-dlen) * sizeof(float));
	}
	delaybuf.Reset();
	outbuf.Reset();
	dlen = (int) newlen;
}

void Deverb::Distort(float d)
{
	dist=d*16384*3.0;
}
float Deverb::getDistort() { return dist/(16384.0*3.0);}

void Deverb::calc_disttbl()
{
	float *dbuf = distbuf.GetBuf(65536);
	float xi;
	int i;

	for(i=0;i<65536;i++) {
		xi = i-32768;
		xi = sin(xi*M_PI/32768.0);
		dbuf[i] = xi;
	}
}
float *
Deverb::getBuffer(int len)
{
	return outbuf.GetBuf(len);
}
float *
Deverb::getBuffer()
{
	return outbuf.GetBuf();
}
