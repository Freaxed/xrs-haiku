/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "TrackEnd.h"
#include "Pattern.h"

#include 	<ScrollBar.h>
#include	<ScrollView.h>
#include	<StringView.h>

class PianoEdit;


class PianoControl : public	TrackEnd {
	
	typedef	TrackEnd  _inherited;

public:
	
						PianoControl(BRect	frame, char*	 name);
		  	void		Reset(Pattern* p);
		 	void 		Draw(BRect r);
 			void		Expand();
 			void		UnExpand();
 			float		getExpansionSize() { /*WARNING */return 112.0;};
			void		MessageReceived(BMessage*);
			void		Refresh();
private:

		Pattern*				curPattern;		
		BScrollView*			scroll_view;
		PianoEdit*				pe;
		BBitmap*				piano;
		BScrollBar*				ve;
		BStringView*			w_note;
		void					setNote(int n);
		char					infonote[5];
		
};

