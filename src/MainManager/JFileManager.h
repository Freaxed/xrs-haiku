/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _JFILEMANAGER_H_
#define _JFILEMANAGER_H_

#include  <File.h>
#include <NodeInfo.h>


class Song;
class SJFilter; 
class TrackManager;
class BCheckBox;
class BFilePanel;

#define	X_REFS_RECEIVED	'xrec'

class Track;


class JFileManager
{
	public:
		
			~JFileManager();		
	static   JFileManager*		Get();
	
	status_t	SaveFile(entry_ref, Song* );
	status_t	LoadFile(entry_ref, Song* );
	
	Song*			EmptySong();
	
	void			Init();	
	status_t		Load();
	void			Save(Song*,bool);
	void			SaveReq(entry_ref,const char*,Song*);

	
	status_t		RenderAsWave(BMessage *setting, Song* song);
	void			ExportWave(BMessage *info);
	
	void			AddMime(BNode*, const char* mime = "audio/XRS-File");
	
	
	int		open_type;
		
	status_t	AskForClose(Song*);
	
	void		AnalizeError(BMessage*);
	BMessage*	ErrorsLog(){ return errors_log;}
	
	
	private:
					JFileManager();
	
		void		ErrorMsg(const char *msg,const char *txt);

		int64		ReadFixed(int64*);
		int64		ReadVar(void*);
		
	
		BFile*			file;
		char*			x;
		
		
		BFilePanel*	filepanel;
		BFilePanel*	openpanel;


		SJFilter 	*filtro;
		TrackManager*	tm;
		BDirectory 	*dir;

		BCheckBox	*chek;

		BMessage*		errors_log;
		BMessage*		compatible; //for loading 1.2 files..
		entry_ref sl;
};


#endif
//--
//--
