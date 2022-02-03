/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MediaFileSaver_H
#define MediaFileSaver_H


#include <SupportDefs.h>

#include <SupportDefs.h>
#include <sndfile.h>
#include "Log.h"



class  MediaFileSaver{
public:
			MediaFileSaver(uint8 channels = 2):audioFile(NULL) { 
				memset((void*)&sfinfo, 0, sizeof(SF_INFO)); 
				sfinfo.samplerate = 44100.0f;
				sfinfo.channels   = channels;
				sfinfo.format  = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
			};
			~MediaFileSaver() {
					Close();
			}
			
			status_t Status() { return audioFile ? B_OK : B_ERROR; }
			
			void	Close() {
				if (audioFile)
					sf_close(audioFile);
				audioFile = NULL;
			}
			
			uint32 WriteBlock(float* data, uint32 frames) {
				return sf_writef_float(audioFile, data, frames);
			};
			
		status_t Open(const char* filepath) {
				LogInfo("Opening file [%s] for saving", filepath);
				if (sf_format_check(&sfinfo) == 0)
				{
					LogError("Invalid Info details");
					return B_ERROR;
				}
				audioFile = sf_open(filepath, SFM_WRITE, &sfinfo);
				if (audioFile == NULL) {
					LogError("Error opening file. [%s]", sf_strerror(NULL));
					return B_ERROR;
				}
				
				return B_OK;
		}
private:

			SF_INFO  	sfinfo;
			SNDFILE* 	audioFile;

};


#endif //  MediaFileSaver_H
