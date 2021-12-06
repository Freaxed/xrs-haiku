/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <View.h>

class TickView : public BView
{
	public:
		
					TickView(BRect);
		  void 		SetTick(int,int,int);
		  void		SetNumberNotes(int);
		  
	      void 		Draw(BRect);
	      void 		AttachedToWindow();
	      void		MessageReceived(BMessage*);
	
	private:
			
			int tick,num_notes;
			BRect	TRect(int d);
};

//-
