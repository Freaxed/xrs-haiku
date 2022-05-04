/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <Box.h>
#include <Bitmap.h>

class TickView : public BBox
{
	public:
		
					TickView(BRect);
		  void 		SetTick(int);
		  void		SetNumberNotes(int);
		  
	      void 		Draw(BRect);
	      void 		AttachedToWindow();
	      void 		DetachedFromWindow();
	      void		MessageReceived(BMessage*);

	
	private:
			
			int32 		tick, num_notes;
			BRect		TRect(int d);
			BBitmap*	b_tick;
};

//-
