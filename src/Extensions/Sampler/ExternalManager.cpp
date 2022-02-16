/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"ExternalManager.h"
#include	"XHost.h"
#include 	"Sample.h"
#include 	"MediaFileLoader.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <File.h>
#include <Entry.h>
#include <Path.h>
#include <Alert.h>
#include "Xed_Utils.h"


ExternalManager::ExternalManager() 
{
	lastStatus=B_OK;
}

ExternalManager::~ExternalManager()
{
	MakeEmpty();
}
void
ExternalManager::MakeEmpty()
{
	LogDebug("ExternalManager::MakeEmpty()");
	Sample	*sample;
	while(CountItems()){
		sample = getSampleAt(0);
		samples_list.Erase(0);
		delete sample;
	}

	samples_list.MakeEmpty();
}

status_t
ExternalManager::InitCheck(){
	return lastStatus;
}

status_t
ExternalManager::AddSample(entry_ref ref, int *pos)
{
	assert(pos != NULL);
	
	for(int i=0;i<samples_list.Count();i++)
	{
		if(strcmp((samples_list[i])->GetName(), ref.name) == 0)
		{
			LogInfo("Samples %s already foud at position %d\n",ref.name, i);
			*pos=i;
			return B_OK;
		}
	}

	Sample* sample = new Sample();

	BAlert*	waitAlert = XUtils::ShowIdleAlert("Loading sample..");
	
	lastStatus  = LoadFile(&ref, sample);
	
	XUtils::HideIdleAlert(waitAlert);
	
	if (lastStatus != B_OK) {
		LogError("Can't open sample file [%s]", ref.name);
		delete sample;
		return lastStatus;
	}
	
	samples_list.Add(sample); 
	*pos = samples_list.Count()-1;
	return B_OK;
}

status_t
ExternalManager::LoadFile(entry_ref *ref, Sample* sample)
{
	BPath	path(ref);
	MediaFileLoader	loader;
	if (loader.Open(path.Path()) != B_OK) {
		return B_ERROR;
	}
	
	const SF_INFO&  sfinfo = loader.Info();
	
	/* SAMPLE SetUp */
	
	float freq_divisor = 44100.0f / (float)sfinfo.samplerate;

	int channels = sfinfo.channels;
	uint32 fullframes = ceil((float)sfinfo.frames * freq_divisor);

	sample->SetPath(path);
	sample->CreateBuffer(fullframes, channels);

	/******/
	
	int   readcount  = 0;
	int32 position   = 0;
	while ((readcount = loader.ConvertNextBlock()) > 0)
	{	
		float *buf = loader.ConvertFreqBuffer();
		for (int k = 0 ; k < readcount ; k++)
		{
			for (int m = 0 ; m < channels ; m++) {
				sample->wave_data[m][position] = buf[k* channels + m];
			}
			position++;
		};
	};
	
	return B_OK;
}

Sample*
ExternalManager::getSampleAt(int val){
	return samples_list[val];
}



SampleList*
ExternalManager::getSampleList(){
	return &samples_list;
}

int32
ExternalManager::CountItems(){
	return samples_list.Count();
}
