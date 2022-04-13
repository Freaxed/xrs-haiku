/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _X_PANEL_
#define  _X_PANEL_


#include "GlobalDef.h"

#include <PopUpMenu.h>
#include <Box.h>

class Song;
class BankManager;
class XDigit;
class Pattern;
class TracksPanel;
class ADigit;
class XPot;
class BPictureButton;
class PeakView;

class XPanel : public BBox
{
	public:
						XPanel (BRect);
						
 		void		MessageReceived(BMessage* message);
		void		AttachedToWindow();
		void		ResetToSong(Song *,TracksPanel*);
		void		KeyDown(const char *bytes,int32);
		void		KeyUp(const char *bytes,int32);
		bool		isAllPat();
			
		void		OnPlay(bool isPlaying);
		void		ResetMeasureCount();

	private:
	
		void	SelectTrack(int);
		PeakView* Two;
	
		ADigit				*curpat;
	
		BPictureButton 		*play_bt;
		BPictureButton 		*all_bt;
		BPictureButton		*edit;
		
		
		Song*			curSong;
		XPot*		 	master;
		char			ttext[4];
		bool 			key_rel;
		
		BPopUpMenu*		bpm_menu;
		TracksPanel*	tp;
		
		BPictureButton*	tool[15];
};
#endif

