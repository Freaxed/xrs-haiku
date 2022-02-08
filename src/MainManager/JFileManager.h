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
		
	static JFileManager*	Get();
	
	status_t	SaveFile(entry_ref,Song*,bool zip=false); 	//The name	??
	status_t	LoadFile(entry_ref,Song*);	//The name    ??
	
	
	Song*			EmptySong();
	void			CloseSong(Song*);	
	
	void			Init();
	
	status_t		Load();
	void			Save(Song*,bool);
	void			SaveReq(entry_ref,const char*,Song*);

	
	status_t	RenderAsWave(BMessage *setting);
	void			ExportWave(BMessage *info);
	
	void		AddMime(BFile*);
	
	
	int		open_type;
		
	status_t	AskForClose(Song*);
	
	void			AnalizeError(BMessage*);
	BMessage*	ErrorsLog(){ return errors_log;}
	
	
	private:
					JFileManager();
	
		void		CollectData(BMessage*,entry_ref);
		void		ErrorMsg(const char *msg,const char *txt);
		void		WriteFixed(int64,int64);
		int64		ReadFixed(int64*);
		void		WriteVar(int64,void*,int64);
		int64		ReadVar(void*);
		
		void		Comatible12(Track*,int32 va,int32 rt);
		
		BFile*			file;
		char*			x;
		
		
		BFilePanel*	filepanel;
		BFilePanel*	openpanel;


		SJFilter 	*filtro;
		TrackManager*	tm;
		BDirectory 	*dir;

		BCheckBox	*chek;
		//bool		allpat;
		//int		poz;
		BMessage*		errors_log;
		BMessage*		compatible; //for loading 1.2 files..
		entry_ref sl;
		

};

//deprecated extern JFileManager*	jfm;

#endif
//--
//--
