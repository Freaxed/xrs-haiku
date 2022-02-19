#include "SettingsTextView.h"

#define COLOR 255,255,255  //240,240,96

SettingsTextView::SettingsTextView(BRect rect, const char *name,BString testo) 
	: BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW){
		
	fText=testo;
	SetViewColor(COLOR); 
}

void SettingsTextView::AttachedToWindow(){
		SetFontSize(12);
}

void BPDrawString(const char* string, BView* parent, BRect rect,alignment ali,float kTEXT_MARGIN = 0.0f)
{
	float		width = rect.Width() - (2 * kTEXT_MARGIN);
	float		y;
	BFont		font;
	font_height	finfo;

	parent->GetFont(&font);
	font.GetHeight(&finfo);
	y = rect.top + ((rect.Height() - (finfo.ascent + finfo.descent + finfo.leading)) / 2) +
					(finfo.ascent + finfo.descent) - 2;

	switch (ali)
	{
		case B_ALIGN_LEFT:
			parent->MovePenTo(rect.left + kTEXT_MARGIN, y);
			break;

		case B_ALIGN_CENTER:
			parent->MovePenTo(rect.left + kTEXT_MARGIN + ((width - font.StringWidth(string)) / 2), y);
			break;

		case B_ALIGN_RIGHT:
			parent->MovePenTo(rect.right - kTEXT_MARGIN - font.StringWidth(string), y);
			break;
			
		default:
			break;
	}
	parent->DrawString(string);
}


void SettingsTextView::Draw(BRect updateRect)
{
//		MovePenTo(BPoint(10,25));
		SetHighColor(0,0,0);
		SetLowColor(COLOR); 
		
		//Utils.cpp function:	
		BPDrawString(fText.String(),this,Bounds(),B_ALIGN_CENTER);
//		DrawString(fText.String());
}
