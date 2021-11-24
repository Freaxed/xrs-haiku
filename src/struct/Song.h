/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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
					Song ();
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
					
		void		setTempo(int bpm); //in bpm 
		int			getTempo();
						
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
		
		BMessage*		mixer_settings;
		
		const char*		getDescription() {return description.String(); };
		void			setDescription(const char*d ){description.SetTo(d);};
		size_t			getSizeDescription(){return description.Length();};
			
	private:

			BList			trk;
			BString			description;
			Sequence*		sequence;
			size_t			note_size;
			int				tempo_bpm;
			BEntry*			file_entry;	
			bool			modified;
			int				num_notes;
};

#endif
