#include "vca303.h"

VCA303::VCA303()
{ 
	mode=0; 
	a=0; 
	attack=1.0-0.94406088; 
	decay=0.99897516; 
	a0=0.5;
}

VCA303::~VCA303()
{}

void VCA303::Process(const float *buf, int len, int channels)
{
	float *out = sbuf.GetBuf(len);
	int i;
	if(!output) return;
	len*=channels;
	for(i=0;i<len;i++) {
		out[i] = buf[i]*a;
		if(!mode) a+=(a0-a)*attack;
		else if(mode==1) {
			a*=decay;
			if(a<(1/65536.0)) { a=0; mode=2; } // prevent underflow exceptions
		}
	}
	
	output->Process(out, len, channels);
}

void VCA303::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case NoteOn:
			mode=0;
			break;
		case NoteOff:
			mode=1;
			break;
		case SystemReset:
			sbuf.Reset();
			break;
		default: break; // gcc is dumn
	}

	if(output) output->Message(msg, data);
}

