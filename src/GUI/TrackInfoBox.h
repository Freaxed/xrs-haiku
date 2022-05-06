/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */
#ifndef _H_TrackInfoBox_h
#define _H_TrackInfoBox_h


#include <SupportDefs.h>
#include <Box.h>
#include <StringView.h>
#include <CheckBox.h>
#include <Message.h>

class		Track;
class		XDigit;

class TrackInfoBox  : public BBox {
public:
        TrackInfoBox(BRect r);

		void	MessageReceived(BMessage*);
    	void	SetTrack(Track* tr);
        void    RegisterPanel(BView*);

private:
		Track*  	fTrack;	
#ifndef XRS_MIDI
		XDigit* 	fMidiChannel;
		BCheckBox*	fMidiEnabled;
#endif
};


#endif // _H_TrackInfoBox_h
