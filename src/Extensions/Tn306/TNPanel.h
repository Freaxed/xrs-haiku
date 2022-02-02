/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TNPanel_H
#define _TNPanel_H

#include "PlugPanel.h"
#include <Menu.h>

class TB303Knobs;
class TNTrack;
class TNTrackBoost;

class TNPanel : public PlugPanel
{
	public:
	
							TNPanel(TNTrackBoost* booster);
			 		void	AttachedToWindow();
					void	MessageReceived(BMessage* msg);
					void	Reset(Track *track);
		
	private:

			TB303Knobs*		knobs;
			TNTrack*		fTrack;
			TNTrackBoost*	fBooster;
			
			BMenu*		presetz;
			BMenu*		men;
};
#endif

