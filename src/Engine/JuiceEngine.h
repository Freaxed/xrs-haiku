/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
#include 	"CommonValuableID.h"

class Song;
class XHost;
class Track;

class JuiceEngine : public Engine, public Tickable {

	public:
	
		
				static JuiceEngine*	Get(); // there can be only one! :)
				 
						~JuiceEngine();
		
				void	ResetSong(Song* song);
				
				//starting events..
				void	TickedHigh(uint64 time,int16 beat,int16 tick);
			
				//stopping events..
				//void	TickedLow(uint64 time,int16 beat,int16 tick);
				
				void	SetLoopEnable(bool enable);
				
				void	MessageReceived(BMessage* msg);
				
				//number of fullframes a single beat is composed of.
				size_t	GetSamplesPerBeat() { return fSamplesPerBeat; }

				void	OnNewTrack(Track* trk); //prepare a newly created track (reset+settempo)
		
	protected:
	friend class JFileManager;
	
					JuiceEngine(const char* name);
			void	Starting();
			void	Stopping();
			void	SecureProcessBuffer(void * buffer, size_t size);
		 	void	SendTrackMessage(SynthMessage msg, float data);
		 	void	ValuableChanged(BMessage* msg);
	
	protected:
		friend	class XHost;
				BList 	rem_list;
					
	private:

			void	SetBPM(int bpm);
			void	UpdateMeters();
						
			float *stream_note[2];
			float *stream_stream[2];
			
			Song*	fCurrentSong;

			size_t	fSamplesPerBeat;
			size_t	fSamplesPerTick;			
			
			void	process_row(int32 row);
			
			size_t	BufferPosition;
			
			//Events
			void			AddVoice(Track*,XRSVoice);
			void			RemoveVoices(Track*);
			void			DeleteVoices(Track*);		
			
			float fPeakLeft[MIXERLINES_COUNT];
			float fPeakRight[MIXERLINES_COUNT];	

};

//deprecated extern JuiceEngine*	msucco;

#endif
