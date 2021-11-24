#ifndef __SYNTHELEMENT_H
#define __SYNTHELEMENT_H

#ifndef NO_NAMESPACES
#include <list>
using namespace std;
#else
#include <list.h>
#endif
#include"stdio.h"

#include "synthMessages.h"

class SampleBuf
{
private:
	float *buf;
	int alloclen;
public:
	SampleBuf() { buf=NULL; alloclen=0; }
	virtual ~SampleBuf();

	float *GetBuf(int size);
	float *GetBuf() { return buf; }
	void	Reset();
};


class Element
{
protected:
	list<Element*> inputs;
	Element *output;

// These are called when another element sets output to this element.
// This is so mixers know just how many things they're mixing.
// The base class uses these for housekeeping in case the chain is broken
// in the middle by deleting an element which another element outputs to.
	virtual void 	AddInput(Element *in);
	virtual void 	RemoveInput(Element *in);
	virtual void 	RemoveOutput(void);
public:
				Element() { output = NULL; }
	virtual 		~Element();
	
	void 			SetOutput(Element *out);

// Process does whatever processing the filter does.
// Channel samples are interleaved in the buffer.
// If channels>1, expect filters to only operate on first channel.
// Synthesizers with no input get a buf=NULL and len=desired output buffer
// size.
	virtual void Process(const float *buf, int len, int channels);
// Flush the output for additive elements (multi-input filters, mixers)
	virtual void Flush(void);
// Control messages sent from high-level elements and propagated through outputs.
	virtual void Message(SynthMessage msg, float data);


};
#endif
