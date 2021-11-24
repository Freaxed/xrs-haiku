/*
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_WindowManager_h_
#define	_WindowManager_h_

#include <Window.h>
#include <List.h>
#include <String.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <MessageFilter.h>

#define	WINSTATE	'wins'

class	WindowManager: public BList, public BMessageFilter 
{
	public:
				~WindowManager();				
		static	WindowManager* Get();
		
		filter_result 	Filter(BMessage *message, BHandler **target);

		void		RegisterMe(BWindow*,const char *str);
		void		UnregisterMe(BWindow*);
		
		void		Switch(int32 p);
		void		Switch(BWindow* w);
		
		void		Show(BWindow* w);
		void		Hide(BWindow* w);
		void		CloseAll();
		
		BPopUpMenu*	getMenu();
		
	private:
	
						WindowManager();
			void		_switch(BWindow*, int32 pos);
			void		dump();
			
			struct token
			{
				BString		reg_name;
				BWindow*	pointer;
			};
		
			BPopUpMenu*	menu;
};	



#endif
