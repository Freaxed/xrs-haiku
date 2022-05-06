/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_TRACKINFOWINDOW_H_
#define	_TRACKINFOWINDOW_H_


#include 	"XrsWindow.h"
#include	<Box.h>
#include	<StringView.h>
#include	<CheckBox.h>
#include	<Message.h>

class TrackInfoBox;
class Track;

class	TrackInfoWindow : public XrsWindow
{
	public:
					~TrackInfoWindow();
			
			static	TrackInfoWindow*	Get();
			
			bool	QuitRequested();
			void	SetTrack(Track* tr);
	        void    RegisterPanel(BView*);
		
	private:
				TrackInfoWindow();
				TrackInfoBox*	fTrackInfoBox;
};
#endif
//--
