/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef PlugWindow_H
#define PlugWindow_H
 
#include <Window.h>

class 	BMenu;
class 	BMenuItem;
class	VSTItem;
class	VSTConfigureView;

class PlugWindow : public BWindow
{



	 public:
							PlugWindow(VSTItem*,bool scroll=false);
							~PlugWindow();
			virtual bool 	QuitRequested();
			void			SetState(bool,float force=-1);
			bool			GetState() { return big;}
			void			SavePref(BMessage*);
			void			LoadPref(BMessage*);		
			virtual void	MessageReceived(BMessage*);
			void			SetPrograms(const char *name,BMenu*);
			void			SetControls(BView*,bool);
			void			SetName();
	private:
			
			BMenu*	presetz;
			bool	big;
			
			BMenuBar *bar;
			BMenuItem* nameMenu;
			VSTConfigureView	*config;
			float	smallW,smallH;
			float	bigW,bigH;
			
	protected:	
			VSTItem*	plugin;
			
};

#endif
