/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SONG_H_
#define _SONG_H_ 

#include "GlobalDef.h"

#include <SupportKit.h>
#include <List.h>
#include "Resolution.h"

class Track;
class BEntry;
class Sequence;
class BString;

class Song 
{
	public:
					 Song();
					~Song();
					
		void			Init();
				
		void		AddTrack(Track*);
		void		RemoveTrack(Track*);
		int			MeltTrack(Track*);			
		Track*		getTrackAt(int);
		int			getNumberTrack();
		int			getIndexOfTrack(Track* tx);
		
		int16		getNumberNotes(){return fBeats * fBeatDivision;};

		void		setBeatInfo(int16 beats, int16 divison);

		
		//////////////////////////////////////////////////////////////////
		// tempo must be an int32 (it's also a Valuable)
		void		setTempo(int32 bpm);
		int32		getTempo();
		//////////////////////////////////////////////////////////////////
						
		BEntry*		getEntry();
		void		setEntry(BEntry*);
				
		// Pattern
		
		void		AddMeasure();
		status_t	RemoveMeasure(int num);
		int			getCurrentMeasure();		
		int			getNumberMeasure();
		void		setNumberMeasure(int);
		
		// Sequence
		
		Sequence*	getSequence(){return sequence;};
		
		// filter

		bool			popupdesc;
		
		const char*		getDescription() 			 { return description.String(); };
		void			setDescription(const char*d ){ description.SetTo(d);        };
		size_t			getSizeDescription()         { return description.Length(); };
		
		int16			GetBeatDivision();
		int16			GetBeats();
	
	private:


			BList			trk;
			BString			description;
			Sequence*		sequence;

			BEntry*			file_entry;	
			bool			modified;

			int16			fBeats;				// by default 4 Beats
			int16			fBeatDivision;  	// by default a beat is divided in 4
			int32			fBeatsPerMinutes;	// BPM / Tempo
};

#endif
