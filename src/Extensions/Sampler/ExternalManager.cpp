/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"ExternalManager.h"
#include 	"file_msg.h"
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


ExternalManager::ExternalManager() 
{
	lastStatus=B_OK;
}

ExternalManager::~ExternalManager()
{
	Empty();
}
void
ExternalManager::Empty()
{
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

	if(pos!=NULL) {
		
		for(int i=0;i<samples_list.Count();i++)
			if(strcmp((samples_list[i])->name,ref.name)==0)
			{
					LogInfo("Samples %s already foud at position %d\n",ref.name,i);
					*pos=i;
					return B_OK;
			}
	}
	
	Sample* sample = new Sample();
	lastStatus  = LoadFile(&ref, sample);
	if (lastStatus != B_OK) {
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
	
	sample->channels = sfinfo.channels;
	sample->freq_divisor= 44100.0f / (float)sfinfo.samplerate;

	sample->name.SetTo(path.Leaf());
	sample->path_name.SetTo(path.Path());
	
	uint32 frames = ceil((float)sfinfo.frames * sample->freq_divisor);
	sample->_totalbytes = frames * sizeof(float) * sample->channels;	
	sample->type = EXT_SAMPLE_TYPE;
	
	for (int m = 0 ; m < sample->channels ; m++) {
		sample->wave_data[m] = new float[frames];
	}
		
	if (sample->channels == 1)
		sample->wave_data[1] = sample->wave_data[0];
		
	sample->fullframes = frames;

	/******/
	
	int   readcount  = 0;
	int32 position   = 0;
	while ((readcount = loader.ConvertNextBlock()) > 0)
	{	
		float *buf = loader.ConvertFreqBuffer();
		for (int k = 0 ; k < readcount ; k++)
		{
			for (int m = 0 ; m < sample->channels ; m++) {
				sample->wave_data[m][position] = buf[k* sample->channels + m];
			}
			position++;
		};
	};
	
	return B_OK;
}



status_t
ExternalManager::AddBankSample(BMessage* message)
{
	entry_ref	ref;
	
	message->FindRef("refs", &ref);
	
	int32 spiaz=message->FindInt32("spiaz");
	//int32 size=message->FindInt32("size");
	
	lastStatus=ExtractSample( ref,spiaz );
	return lastStatus;
}

status_t
ExternalManager::ExtractSample(entry_ref ref ,int32 spiaz )
{
	//Lock the main thread:
	XHost::Get()->SendMessage(X_LockSem,0);
	lastStatus = _extractSample(ref,spiaz);
	//Unlock the main thread:
	XHost::Get()->SendMessage(X_UnLockSem,0);
	return lastStatus;	
}

status_t
ExternalManager::_extractSample(entry_ref ref ,int32 spiaz )
{	
/*
	status_t error;
			
	BEntry controllo_esiste(&ref);
	if(controllo_esiste.Exists()==false) return B_ENTRY_NOT_FOUND;
	
	
	BFile *file=new BFile();
    	
    	error=file->SetTo(&ref,B_READ_ONLY);
    		if(error) return error;
    	error=file->InitCheck();
    		if(error) return error;
	
	//error=file->GetSize(&file_size);
    	//	if(error) return error;
	
	Sample* samp=new Sample();
	
	int64 msg=-1;
	int64 val=0;
	char title[30];
	char space=' ';
	
	
	
	file->Seek(spiaz,SEEK_SET);
	
	while(msg!=SAMPLE_STOP)
			{
				file->Read(&msg,sizeof(int64));
				file->Read(&val,sizeof(int64));
				
				
				switch(msg){
				case SAMPLE_NAME:
					
					file->Read(title,val);
					memset(sample->name,space,30);
					memcpy(sample->name,title,val);
					sample->name[val]='\0';
				
					
					break;
				case SAMPLE_DATA:
					
					sample->wave_data=(short*)malloc(val);
					sample->_totalbytes=val;
					sample->frames=val/2;
					sample->channels=2;
					sample->type=BANK_SAMPLE_TYPE;
					file->Read((void*)sample->wave_data,val);
					break;
					
					break;
				default:
					if(msg>=800)
					{
						//printf("Unknown code :%lld\n",msg);
						file->Seek(val,SEEK_CUR);
					}
				break;
				}
	
	}
	

	
	samples_list.Add(samp);	 
	
	BEntry e(&ref);
	BPath p(&e);
	
	sample->path_name=(char*)malloc(B_PATH_NAME_LENGTH);
	memcpy( sample->path_name, p.Path(), strlen(p.Path() ) );
	sample->path_name[strlen(p.Path() )]='\0';
	sample->offset=spiaz;
	
	printf("External File loaded! [%s]\n",sample->path_name);	
	
	delete file;*/
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
