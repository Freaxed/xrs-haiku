/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _VOLVIEWL_
#define  _VOLVIEWL_

#include "TrackEnd.h"
#include "Pattern.h"

enum {

	VELOCITY_VIEW=0,
	PAN_VIEW,
	SUSTAIN_VIEW
};

class Note;
class BPopUpMenu;

class VolView : public TrackEnd
{
	public:
	
					VolView (BRect);
	
			void		Draw(BRect);
	 		void		MouseDown(BPoint);
	 		void		MouseUp(BPoint);
	 		void 		MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
			void		Reset(Pattern*);
			void		ModeView(int32 mode);
			void		MessageReceived(BMessage*);
			void		AttachedToWindow();
			void		SupportMultiMode(int maxv=PAN_VIEW); //turn on multimode				
			
	private:

		bool			multimode;
		Pattern*		curPattern;
		int				prev;
		bool			track;
		int32			mode_view;
		void			_drawNote(int);
		void			_setValue(Note*,float);
		void			_resetPar();
		void			_randPar();
		BPictureButton*	change;
		BPictureButton*	tools;
		rgb_color		back;
		int				maxview;
		

};
#endif

