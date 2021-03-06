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
#include "JFileManager.h"

class Juice;
class Pattern;
class BList;
		//managers
class		WindowManager;
class		VstManager;
class		TrackManager;
class		MeasureManager;
class		JFileManager;		
		//others
class		XHost;
class		PMixer;
class		JuiceEngine;
		//windows
class		MixerWindow;
class		MainWindow;
class		MatrixWindow;
class		MBWindow;
class		TrackInfoWindow;	
class		ValuableManager;
		//model	
class		BasicModelManager;
class		ValuableMonitorWindow;

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
		VstManager*			vst_manager;
		TrackManager*		track_manager;
		MeasureManager*	 	mea_manager;
		JFileManager*		jfm;		
		//others
		XHost*				xhost;
		PMixer*				a_mixer;
		JuiceEngine*		msucco;
		//windows
		MixerWindow*		mixerWin;
		MainWindow*			main_window;
		MatrixWindow*		mw;
#ifdef MEDIA_BROWSER
		MBWindow*			media_browser;
#endif
		TrackInfoWindow*	trackinfo;	
		ValuableManager*	fVManager;	
		BasicModelManager*	fModel;
		ValuableMonitorWindow* fValuableMonitor;
		
};
