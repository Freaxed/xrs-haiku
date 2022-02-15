/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
 
#include 	"XrsWindow.h"
#include 	"Panels.h"
#include	"TracksPanel.h"
class Song;
class XPanel;
class BMenu;
class Pattern;
class TickView;
class Track;



class MainWindow : public XrsWindow
{
	 public:
						
							~MainWindow();						
				static		MainWindow*	Get();
				
				void		Close();
				void		MessageReceived(BMessage* message);
				void		ResetToSong(Song*);
				void		Init();
				bool		QuitRequested();
				void		WindowActivated(bool active);
				void		Saved();
						
				void		PlayButtonOn(bool state);

				
				void		SetWheelTarget(BHandler*);
				
	protected:
	
	friend class TracksPanel;
			void		AddTrack(Track*);
	
	
	private:
								MainWindow();
			status_t			AskRemove(const char*);
			void				CreateMenu();
			void				UpdateRecents();

			void				ResetTitle();
			
			XPanel 			*fPanel;
			Song			*curSong;
			TracksPanel		*fTracksPanel;						
			TickView		*ticks;
						
			/*Up Menu */
			BMenuItem	*saveas_item;
			BMenuItem	*save_item;	
			BMenuBar	*menuBar;
			BMenu		*menuFile;
			BMenu		*openRecent;
			BMenuItem	*copy;
			BMenuItem	*cut;
			BMenuItem	*paste;
			BMenuItem	*rename;
			BMenuItem	*mea_copy;
			BMenuItem	*mea_cut;
			BMenuItem	*mea_paste;
			BHandler	*wheel_target;
			
			
};

//extern MainWindow*		main_window;

#endif
