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

#include <File.h>
#include <NodeInfo.h>
#include <FilePanel.h>

class Song;
class BFilePanel;

#define	X_REFS_RECEIVED	'xrec'

class Track;

class SJFilter : public BRefFilter
{
public:
		SJFilter(){};
		
		bool Filter(const entry_ref*, BNode* node, struct stat_beos*, const char* mimeType)
		{
			return ( node->IsDirectory() || (strcmp(mimeType,"audio/XRS-File") == 0));
		}
};



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
	
	status_t		AskForClose(Song*);
	
	
	
	private:
					JFileManager();
		
		BFilePanel*	filepanel;
		BFilePanel*	openpanel;
		SJFilter*		filtro;
};


#endif
//--
//--
