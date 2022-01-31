/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MediaFileLoader_H
#define MediaFileLoader_H


#include <SupportDefs.h>



#define	BLOCK_SIZE 4096
class MediaFileLoader
{
	public:
			MediaFileLoader():audioFile(NULL), srcState(NULL), fullframes(0), freqFactor(1.0f) { 
				memset((void*)&sfinfo, 0, sizeof(SF_INFO)); 
			};
			
			float*	ReadBuffer() {
				static	MediaFileLoader::StaticBuffer readBuffer;
				return readBuffer.Buffer();
			}
			
			float*	ConvertFreqBuffer() {
				static	MediaFileLoader::StaticBuffer convertBuffer;
				return convertBuffer.Buffer();
			}
			
			
			status_t Open(const char* filepath, float targetFrequency = 44100.0f) {
								
				
				LogInfo("Opening file [%s]", filepath);
				audioFile = sf_open(filepath, SFM_READ, &sfinfo);
				if (audioFile == NULL) {
					LogError("Error opening file. [%s]", sf_strerror(NULL));
					return B_ERROR;
				}
				
				fullframes = BLOCK_SIZE / sfinfo.channels;
				
				if (sfinfo.channels > 2 || sfinfo.channels < 1) 
				{
					LogInfo("Samples with more thann 2 channels are not supported!\n");
					return B_ERROR;
				}
				int error = 0;
				srcState = src_callback_new(MediaFileLoader::src_callback, SRC_SINC_BEST_QUALITY,  sfinfo.channels, &error, this);
				if (srcState == NULL || error != 0)
				{
					LogError("Error creating frequency converter! (error %d)\n", error);
					return B_ERROR;
				}
				
				freqFactor = targetFrequency / (float)sfinfo.samplerate;
				
	
				return B_OK;
			}
			
			
			int	ConvertNextBlock()
			{
				return (int) src_callback_read (srcState, freqFactor, fullframes, ConvertFreqBuffer());
			}
			
			SRC_STATE*	State() { return srcState;}
			
			const SF_INFO& Info() { return sfinfo; }
			
		
			static long src_callback(void *cb_data, float **data)
			{
				MediaFileLoader*	file = (MediaFileLoader*)cb_data;
				*data = file->ReadBuffer();
				return file->ReadFloatBlock();
			}
		
		
		virtual ~MediaFileLoader() {
				if (audioFile)
					sf_close(audioFile);
				if (srcState)
					src_delete (srcState);
		}
		
		
	
	private:
			SF_INFO  	sfinfo;
			SNDFILE* 	audioFile;
			SRC_STATE*	srcState;
			
			struct StaticBuffer {
				public:
				
				float*		buffer;
				
				status_t	Check()  { return buffer ? B_OK : B_ERROR; }
				float*		Buffer() { return buffer; }

				
				StaticBuffer() {
					buffer = (float*)malloc(BLOCK_SIZE * sizeof (float));				
					if (buffer == NULL) {
	  					 LogError ("MediaFileLoader - Error : Out of memory.\n\n") ;
					}
					LogInfo("StaticBuffer ready!");
				}
			};
			
			uint32	 	fullframes;
			float		freqFactor;
			
			int	ReadFloatBlock()
			{
				return (int) sf_readf_float (audioFile, ReadBuffer(), fullframes);
			}
			
};


#endif // MediaFileLoader_H
