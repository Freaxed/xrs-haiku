/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _JuiceEngine_h_
#define _JuiceEngine_h_

#include 	"Engine.h"
#include 	"synthMessages.h"
#include	"Track.h"
#include	"Tickable.h"

class Song;
class XHost;

class JuiceEngine : public Engine, public Tickable {

	public:
	
		
				static JuiceEngine*	Get(); // there can be only one! :)
				 
						~JuiceEngine();
		
				int		GetBPM();
				void	SetBPM(int bpm);
				void	ResetSong(Song* song);
				
				//starting events..
				void	TickedHigh(uint64 time,long beat,long tick);
			
				//stopping events..
				void	TickedLow(uint64 time,long beat,long tick);
		
	protected:
					JuiceEngine(const char* name);
			void	Starting();
			void	Stopping();
			void	SecureProcessBuffer(void * buffer, size_t size);
		 	void	SendTrackMessage(SynthMessage msg, float data);
	
	protected:
		friend	class XHost;
				BList 	rem_list;
					
	private:
			
						
			float *stream_note[2];
			float *stream_stream[2];
			
			Song*	fCurrentSong;

			int32	fSamplesPerBeat;
			int32	fSamplesPerTick;			
			
			void	process_row(int32 row);
			
			int	BufferPosition;
			
			//Events
			void			AddVoice(Track*,XRSVoice);
			void			RemoveVoices(Track*);
			void			DeleteVoices(Track*);		
			
			float fPeakLeft;
			float fPeakRight;	

};

//deprecated extern JuiceEngine*	msucco;

#endif
