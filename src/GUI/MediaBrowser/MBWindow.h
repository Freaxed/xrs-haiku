/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef MBWindow_H
#define MBWindow_H
 
#include "XrsWindow.h"

#include <SimpleGameSound.h>

class BListItem;
class BOutlineListView;

class MBWindow : public XrsWindow
{

public:
					 MBWindow();
					~MBWindow();
			
			bool 	QuitRequested();	
			void 	MessageReceived(BMessage*);
		
private:
			void	AddFolder(entry_ref ref);
			void 	ParseDir(entry_ref*, BListItem*);
			void	_addFolders();
			
			
			void	PlaySelectedSound();
	
	
			BOutlineListView*	fList;
			BSimpleGameSound*	fSound;
			
			BBitmap  *fIwave;
			BBitmap  *fIcon;
};

#endif

//-
