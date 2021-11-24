#include "vco303.h"

void VCO303::Message(SynthMessage msg, float data)
{
	switch(msg) {
		case NoteChange:
			inc=14000.0*data/44100;
			break;
		case NoteOn:
			k=0;
			break;
		case SystemReset:
			out.Reset();
			break;
			
		default: break;
	}
	if(output) output->Message(msg,data);
}

void VCO303::Process(const float *buf, int len, int channels)
{
	float *outbuf = out.GetBuf(len);
	int i;
	
	
	//	SAWTOOTH
	for(i=0;i<len;i++) {
		outbuf[i]=k;
		k += inc;
		if(k>7000) k-=14000;
	}
	
			
	if(output) output->Process(outbuf, len, channels);
}

