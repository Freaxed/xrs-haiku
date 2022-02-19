#include "SettingsTextView.h"

#define COLOR 255,255, 255  //240,240,96

SettingsTextView::SettingsTextView(BRect rect, const char *name, BString testo) 
	: BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW){
		
	fText.SetTo("Prova"); //testo;
	//SetViewColor(COLOR); 
}

void SettingsTextView::AttachedToWindow(){
		SetFontSize(12);
}
#include <stdio.h>
void _BPDrawString(const char* str, BView* parent, BRect rect,alignment ali,float kTEXT_MARGIN = 0.0f)
{
	float		width = rect.Width() - (2 * kTEXT_MARGIN);
	float		y,x;
	BFont		font;
	font_height	finfo;

	parent->GetFont(&font);
	font.GetHeight(&finfo);
	y = rect.top + ((rect.Height() - (finfo.ascent + finfo.descent + finfo.leading)) / 2) +
					(finfo.ascent + finfo.descent) - 2;

	
	switch (ali)
	{
		case B_ALIGN_LEFT:
			x = rect.left + kTEXT_MARGIN;
			break;

		case B_ALIGN_CENTER:
			x = rect.left + kTEXT_MARGIN + ((width - font.StringWidth(str)) / 2);
			break;

		case B_ALIGN_RIGHT:
			x = rect.right - kTEXT_MARGIN - font.StringWidth(str);
			break;
			
		default:
			break;
	}
	parent->MovePenTo(x, y);
	parent->DrawString(str);
}


void SettingsTextView::Draw(BRect updateRect)
{
//	SetHighColor(0,0,0);
//	SetLowColor(COLOR); 
	
	//Utils.cpp function:	
	_BPDrawString(fText.String(), this, Bounds(), B_ALIGN_CENTER);
}
