/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <View.h>
#include "ValuableManager.h"

class TickView : public BView , public ValuableReceiver
{
	public:
		
					TickView(BRect);
		  void 		SetTick(int);
		  void		SetNumberNotes(int);
		  
	      void 		Draw(BRect);
	      void 		AttachedToWindow();
	      void 		DetachedFromWindow();
	      void		MessageReceived(BMessage*);
	      BHandler* GetHandler(){ return this;}
	
	private:
			
			int32 	tick, num_notes;
			BRect	TRect(int d);
};

//-
