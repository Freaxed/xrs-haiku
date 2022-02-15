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
		
		int			getNumberNotes(){return num_notes;};
		void		setNumberNotes(int);
		
		size_t		getNoteSize(); //in frame	
					
		void		setTempo(int32 bpm); //in bpm 
		int32		getTempo();
						
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
			
//TEMP: this is where we save the full information of a file:

		BMessage		fullFile;

	private:

			BList			trk;
			BString			description;
			Sequence*		sequence;
			size_t			note_size;
			int32			tempo_bpm;
			BEntry*			file_entry;	
			bool			modified;
			int				num_notes;
};

#endif
