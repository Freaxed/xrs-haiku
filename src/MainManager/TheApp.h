/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 *
 */

#include <Application.h>

#include "MainWindow.h"
#include "Song.h"

class Pattern;
class BList;
class WindowManager;
class TrackManager;
class MeasureManager;
class JFileManager;		
class JuiceEngine;
class MixerWindow;
class MainWindow;
class MBWindow;
class ValuableManager;
class BasicModelManager;
class ValuableMonitorWindow;

class TheApp : public BApplication {

public:

		
					TheApp(const char * signature);
					~TheApp();
	
			void		PrepareToRun();
			void		MessageReceived(BMessage* message);	
			bool 		QuitRequested();
			void 		RefsReceived(BMessage *);
			void		OpenRefs(BMessage *message);
			void		AboutRequested();
		
			Song*		getSong();

private:

		void					CopyPattern(Pattern*,Pattern*);
		void					ClearPattern(Pattern*);
		
		/* questi lavorano su DefaultSong : */
		Song*					LoadRequest(entry_ref);
		void					EmptySong(bool close);
		void					LoadSong(entry_ref ref);
			
		
		Song*				currentSong;	
				
		/* ClipBoardz */
		Pattern*			clipboard;
		BList*				dinamic_clip;		
		
		
		//managers
		WindowManager*		win_manager;
		TrackManager*		track_manager;
		MeasureManager*	 	mea_manager;
		JFileManager*		jfm;		

		//others
		JuiceEngine*		msucco;

		//windows
		MixerWindow*		mixerWin;
		MainWindow*			main_window;

#ifdef MEDIA_BROWSER
		MBWindow*			media_browser;
#endif

		ValuableManager*	fVManager;	
		BasicModelManager*	fModel;
		ValuableMonitorWindow* fValuableMonitor;
		
};
