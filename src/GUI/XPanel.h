/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _X_PANEL_
#define  _X_PANEL_


#include "GlobalDef.h"
#include "VUView.h"

#include <PopUpMenu.h>
#include <View.h>

class Song;
class BankManager;
class XDigit;
class Pattern;
class TracksPanel;
class XDigit;
class XPot;
class BPictureButton;
class PeakView;

class XPanel : public BView
{
	public:
						XPanel (BRect);
						
 		void		MessageReceived(BMessage* message);
		void		AttachedToWindow();
		void		Reset(Song *,TracksPanel*);
		void		KeyDown(const char *bytes,int32);
		void		KeyUp(const char *bytes,int32);
		bool		isAllPat();
			
		void		PlayButtonOn(bool);
		void		ResetMeasureCount();

	private:
	
		void	SelectTrack(int);
		VUView* One;
		PeakView* Two;
	
		XDigit				*curpat;
	
		BPictureButton 		*play_bt;
		BPictureButton 		*all_bt;
		BPictureButton		*edit;
		BPictureButton		*vst;
		
		
		//XDigit			*bpm;
		Song			*curSong;
		XPot*		 	master;
		char			ttext[4];
		bool 			key_rel;
		
		BPopUpMenu*		bpm_menu;
		TracksPanel*	tp;
		
		BPictureButton*	tool[15];
};
#endif

