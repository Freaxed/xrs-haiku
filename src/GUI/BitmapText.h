#ifndef BitmapText_H_
#define BitmapText_H_

#include <View.h>
#include <Bitmap.h>

class BitmapText : public BView {

	public:
				 BitmapText(BRect rect,const char* text);
			void Draw(BRect);

	private:
			char* fText;
			int		i_len;
			float	p_start,p_end;
			BRect	*rects;
			BBitmap	*sourceBitmap;
};

#endif
//--
