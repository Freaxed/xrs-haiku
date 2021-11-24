/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef  _X_NOTESVIEWL_
#define  _X_NOTESVIEWL_


#include "GlobalDef.h"
#include "Pattern.h"

#include <Control.h>

#define NOTIFY_NOTE 'noty'

class XNotesView : public BControl
{
	public:
	
					XNotesView (BRect,int16);
					~XNotesView();
		void		Draw(BRect);
		void		MouseDown(BPoint);
		void		MouseUp(BPoint);
	 	void 		MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
		void		Reset(Pattern*);

		//void		Reset(Pattern*,bool);
		//status_t	Init();
					

			
	private:

		Pattern*	curPattern;
		BBitmap*	picOn;
		BBitmap*	picOff;
		BBitmap*	picOnB;
		BBitmap*	picOffB;
		int			prev;
		bool		set_state;
		int			selected;
		BMessage	*notify;
};
#endif

