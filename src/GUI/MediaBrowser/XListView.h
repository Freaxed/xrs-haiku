/*
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <OutlineListView.h>

class XListView : public BOutlineListView
{
	public:
				XListView(BRect r);
		bool 	InitiateDrag(BPoint,int32,bool);
//		void	MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
	
	private:
	
		BBitmap* 	createBitmap(const char* txt, BRect *r, BPoint *p, BListItem *s);
		
};
