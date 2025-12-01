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
#include	<LayoutBuilder.h>

class		Track;
class		XDigit;

class	TrackInfoWindow : public XrsWindow
{
	public:

				~TrackInfoWindow();

			static	TrackInfoWindow*	Get();

			bool	QuitRequested();
			void	MessageReceived(BMessage*);
			void	SetTrack(Track* tr);
			void	AddPanel(BView* panel);



	private:
				TrackInfoWindow();

		Track*	myTrack;
		XDigit*	ch;
		BCheckBox*	en;
		BGroupView*	fPanelsContainer;
};
#endif
//--
