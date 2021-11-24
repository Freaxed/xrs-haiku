
#include <stdlib.h>
#include <string.h>
#include "synel.h"
#include"stdio.h"

#ifndef NO_NAMESPACES
#include <algorithm>
using namespace std;
#else
#include <algo.h>
#endif

//////////////////////////////////////////////////////////
// Sample buffer
SampleBuf::~SampleBuf()
{
	if(buf) free(buf);
}

float *SampleBuf::GetBuf(int size)
{
	if(size>alloclen) {
		alloclen=size;
		buf=(float*)realloc(buf, alloclen*sizeof(float));
		Reset();
	}
	
	return buf;
}

void	SampleBuf::Reset()
{
	if(buf)
	{
		memset(buf, 0,alloclen*sizeof(float));
	}
}

//////////////////////////////////////////////////////////
// Synthesizer Element

Element::~Element()
{
	list<Element*>::iterator i;
	for(i=inputs.begin();i!=inputs.end();i++)
		(*i)->RemoveOutput();
	if(output) output->RemoveInput(this);
}

void Element::SetOutput(Element *out)
{
	if(output) output->RemoveInput(this);
	output=out; 
	output->AddInput(this);
}

void Element::AddInput(Element *in)
{
	inputs.push_back(in);
}

void Element::RemoveInput(Element *in)
{
	list<Element*>::iterator i;
	i=find(inputs.begin(), inputs.end(), in);
	if(i==inputs.end()) return;
	inputs.erase(i);
}

void Element::RemoveOutput(void)
{
	output = NULL;
}

void Element::Process(const float*, int, int)
{}

void Element::Flush(void)
{}

void Element::Message(SynthMessage type, float data)
{
	if(output) output->Message(type, data);
}