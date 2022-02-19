/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <Entry.h>

class JFileManager;
class Song;

class Panels
{
	public:
		static	void	showPrefsPanel();
		static	bool	msgPrefs(BMessage* msg,void* cookies);
		
		static	void 	showSettings(Song*);
		static	bool	msgSettings(BMessage* msg,void* cookies);
		
		static	void	showExport(int k,int m);
		static	bool	msgExport(BMessage* msg,void* cookies);
		
		static	void	showErrors(BMessage* msg); 	
};
