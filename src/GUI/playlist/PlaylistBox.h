/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _PLAYLIST_BOX_
#define _PLAYLIST_BOX_

#include <Box.h>
#include <CheckBox.h>
#include <ScrollView.h>
#include <ScrollBar.h>
#include <Menu.h>

class XMatrix;
class XMName;
class Sequence;
class XMPoz;

class PlaylistBox : public BBox
{
    public:
        PlaylistBox(BRect);
        virtual void FrameResized(float,float);
        virtual void MessageReceived(BMessage *m);
        virtual void AttachedToWindow();
        void Reset(Sequence* s, int16 notesPerMeasaure);

    private:
        BCheckBox*   fEnableLoop;
        XMatrix*     the_m;
        XMName*      the_n;
        BScrollView* scroll;
        BScrollBar*  scroll_bar;
        Sequence*    seq;
        int 		 y_count;
        XMPoz*       tt;
        BMenu*		 menuMea;

};

#endif // _PLAYLIST_BOX_
