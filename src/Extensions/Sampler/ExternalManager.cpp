/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"ExternalManager.h"
#include 	"file_msg.h"
#include	"XHost.h"
#include 	"Sample.h"

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
ExternalManager::AddSample(entry_ref ref,int *pos)
{
	if(pos!=NULL)
	for(int i=0;i<samples_list.Count();i++)
		if(strcmp((samples_list[i])->name,ref.name)==0)
		{
				printf("Samples %s already foud at position %d\n",ref.name,i);
				*pos=i;
				return B_OK;
		}
	
	lastStatus = LoadFile(&ref,pos);
	return lastStatus;
}

status_t
ExternalManager::LoadFile(entry_ref *ref, int *pos)
{
	
	uint32			ref_num;
	
	BMediaTrack 	*audTrack(NULL);
	media_format	format;
	memset(&format, 0, sizeof(format));

	int32			channels = 1;
	
	ref_num=0;
	if (ref){

		BMediaFile		inFile(ref);
		
		if (inFile.InitCheck() == B_OK)
		{
     	  char s[B_FILE_NAME_LENGTH +20];
		  sprintf(s, "Xrs - %s", ref->name);
				
			// gather the necessary format information
			int32 tracks = inFile.CountTracks();
			
			for (int32 i = 0; i < tracks; i++) {
			
				BMediaTrack *inTrack = inFile.TrackAt(i);
				inTrack->EncodedFormat(&format);

				if (format.IsAudio()) {
					audTrack = inTrack;
					inTrack->DecodedFormat(&format);

//					memcpy(&Pool.m_format, &format, sizeof(Pool.m_format));
					//Pool.sample_bits = (format.u.raw_audio.format & 0xf)*8;
					//Pool.selection = NONE;
					//Pool.frequency = format.u.raw_audio.frame_rate;
					//Pool.size = audTrack->CountFrames()-1;
					channels = format.u.raw_audio.channel_count;
					int32 frame_size = (format.u.raw_audio.format & 0xf)*channels;
					
					//format.u.raw_audio.frame_rate!=44100 ||
					 if(  channels > 2) 
					   {
					    //FIX.. devo uscire?
					    return B_MEDIA_BAD_FORMAT;
					   }
					printf(" frame_size %ld\n", 
					frame_size					
					);

				}else{
					inFile.ReleaseAllTracks();
					return B_MEDIA_BAD_FORMAT;
				}
			}

			int64 			frameCount, framesRead;
			status_t 		err;
			media_header 	mh;
		
			
			
			frameCount = audTrack->CountFrames();
			
			void *buffer = malloc(format.u.raw_audio.buffer_size);// temp memory 
						
			short *wave_data=	(short*)malloc(frameCount*channels*2);
			memset( wave_data, 0, frameCount*channels*2); 
			
			
			// read audio from source and write to destination, if necessary
			if (wave_data) {
				
				short *mem = NULL;
				mem = wave_data;
				
				int64 count =0;

				for (int64 i = 0; i < frameCount; i += framesRead) 
				{
				
					
					
					// clear buffer first
					memset( buffer, 0, format.u.raw_audio.buffer_size);
					if ((err = audTrack->ReadFrames(buffer, &framesRead, &mh)) != B_OK) {
						printf("Error reading audio frames: %s\n", strerror(err));
						break;
					}

					count += framesRead;			// now correct for crashes if bigger than file
					if (count > frameCount)
						framesRead -= (count - frameCount);
		
					switch(format.u.raw_audio.format){
					case 0x24:	// 0 == mid, -1.0 == bottom, 1.0 == top (the preferred format for non-game audio)
					{	float *tmp = (float*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++){
							x = *tmp++;
							if (x<-1.0)		x = -1.0;
							else if (x>1.0)	x = 1.0;
							*mem++ = (short)(x*327767.0);
						}
					}	break;
					case 0x4:	// 0 == mid, 0x80000001 == bottom, 0x7fffffff == top (all >16-bit formats, left-adjusted)
					{	int32 *tmp = (int32*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++){
							x = *tmp++/0x80000000;
							if (x<-1.0)		x = -1.0;
							else if (x>1.0)	x = 1.0;
							*mem++ = (short)(x*327767.0);
						}
					}	break;
					case 0x2:	// 0 == mid, -32767 == bottom, +32767 == top
					{	int16 *tmp = (int16*)buffer;
						//float x;
						for (int32 count = 0; count<framesRead*channels; count++){
							//x = *tmp++/32767.0;
							//if (x<-1.0)		x = -1.0;
							//else if (x>1.0)	x = 1.0;
							*mem++ = *tmp++; //(short)(x*327767.0);
						}
					}	break;
					case 0x11:	// 128 == mid, 1 == bottom, 255 == top (discouraged but supported format)
					{	uint8 *tmp = (uint8*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++){
							x = *tmp++/127.0 -1.0;
							if (x<-1.0)		x = -1.0;
							else if (x>1.0)	x = 1.0;
							*mem++ = (short)(x*327767.0);
						}
					}	break;
					case 0x1:		// 0 == mid, -127 == bottom, +127 == top (not officially supported format)
					{	int8 *tmp = (int8*)buffer;
						float x;
						for (int32 count = 0; count<framesRead*channels; count++){
							x = *tmp++/127.0;		// xor 128 to invert sign bit
							if (x<-1.0)		x = -1.0;
							else if (x>1.0)	x = 1.0;
							*mem++ = (short)(x*327767.0);
						}
					}	break;
					}
					
					
					
				}
				inFile.ReleaseAllTracks();
				free(buffer);
				
			}else{
				return B_ERROR;
		         //(new BAlert(NULL,Language.get("MEM_ERROR"),Language.get("OK")))->Go();
			}
			
			 //OK, now we should convert the sample.
			
			 //Create the Sample!
			 Sample* samp=new Sample();
		     
		     strcpy(samp->name,ref->name);
			 samp->wave_data=wave_data;
			 samp->_totalbytes=frameCount*channels*2; //always, as soon as we use 16bit sample (2byte)
			 samp->frames=frameCount * channels; //every single frames|
			 samp->type=EXT_SAMPLE_TYPE;
			 samp->channels=channels;
			 samp->freq_divisor= 44100. / (float)format.u.raw_audio.frame_rate ;
	    	 samples_list.Add(samp);	 
	
			 BEntry e(ref);BPath p(&e);
	      	 samp->path_name=(char*)malloc(B_PATH_NAME_LENGTH);
	         strcpy(samp->path_name,p.Path());
	         printf("External File loaded! [%s]\n",samp->path_name);	
			 samp->PrintToStream();
			int zz=samples_list.Count()-1;
			if(pos) memcpy((void*)pos,(void*)&zz,sizeof(int)); //?
			
			return B_OK;
				

		}else{
			return B_ERROR;
			//(new BAlert(NULL,Language.get("LOADING_NO_AUDIO"),Language.get("OK")))->Go();
		}
	}
	return B_MEDIA_BAD_FORMAT;
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

	status_t error;
			
	BEntry controllo_esiste(&ref);
	if(controllo_esiste.Exists()==false) return B_FILE_NOT_FOUND;
	
	
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
					memset(samp->name,space,30);
					memcpy(samp->name,title,val);
					samp->name[val]='\0';
				
					
					break;
				case SAMPLE_DATA:
					
					samp->wave_data=(short*)malloc(val);
					samp->_totalbytes=val;
					samp->frames=val/2;
					samp->channels=2;
					samp->type=BANK_SAMPLE_TYPE;
					file->Read((void*)samp->wave_data,val);
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
	
	/*Sample* samp=new Sample();
	samp->data=(void*)malloc(size);
	file->Read(samp->data,size);
	
	memcpy(samp->name,name,30);
	samp->name[30]='\0';		
	
	samp->totalbytes=size;
	samp->type=BANK_SAMPLE_TYPE;*/
	
	samples_list.Add(samp);	 
	
	BEntry e(&ref);
	BPath p(&e);
	
	samp->path_name=(char*)malloc(B_PATH_NAME_LENGTH);
	memcpy( samp->path_name, p.Path(), strlen(p.Path() ) );
	samp->path_name[strlen(p.Path() )]='\0';
	samp->offset=spiaz;
	
	printf("External File loaded! [%s]\n",samp->path_name);	
	
	delete file;
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
