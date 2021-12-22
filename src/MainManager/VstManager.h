/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_VSTMANAGER_H_
#define	_VSTMANAGER_H_

class	BList;
class	BMenu;
class	BFile;
class 	VSTItem;

#include	<Path.h>
#include    <List.h>

class VstManager
{
	public:
			 
			~VstManager();
		
		static	VstManager*	Get();
		
		BList*		getList(){ return &list;}
		VSTItem*	CreateVst(int pos);	
		void		DeleteVst(VSTItem* plug);
		bool		SavePreset(VSTItem* plug,const char*name,BMessage* msg);
		bool		LoadPreset(VSTItem* plug,const char*name,BMessage* msg);
		
		void		FillPresetsMenu(VSTItem* plug,BMenu*,uint32 msg);
		void		setPresetsPath(VSTItem* plug);
		
			
	private:
	
					VstManager();
		BList		list;
		BPath		vstpath;

		void		AddMime(BFile* file);
		bool		CheckMimeType(BFile* file);
};

#endif
