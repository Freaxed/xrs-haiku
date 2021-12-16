/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include	"MixableBox.h"
#include	"ValuableManager.h"
#include	"PMixable.h"
#include	"GfxMeter.h"
#include	"Xed_Utils.h"

#include <stdio.h>

#define WIDTH 	120
#define HEIGHT	240


MixableBox::MixableBox(BPoint position, BString name, ValuableID volume, ValuableID pan, ValuableID meter)
	:BBox(BRect(position,position + BPoint(WIDTH-1,HEIGHT-1)),
		  "a_mixable_box",B_FOLLOW_NONE,B_WILL_DRAW)
{
	
	//Title
	fName=new FixedText(BRect(5,5,115,25), name);
	fName->SetAlignment(B_ALIGN_CENTER);
	AddChild(fName);
	
	BRect rect(Bounds());
	rect.InsetBy(5,5);
	rect.right = (rect.right-rect.left)/2;
	rect.top = 26;
		
	//Slider
	fSlider = new XChannelSlider(rect, "XChannelSlider", volume, B_VERTICAL);
	fSlider->SetLimitLabels(NULL,NULL);
	fSlider->SetHashMarks(B_HASH_MARKS_RIGHT);
	fSlider->SetHashMarkCount(16);
	AddChild(fSlider);

	//Pot
	rect.OffsetTo(rect.right + 1,rect.top);
	rect.bottom=rect.top+22;
	rect.right=rect.left+22;
	fPot = new XPot(rect, "XPot", pan, -100, +100, XUtils::GetBitmap(24), NULL);
	AddChild(fPot);
	
	//Meter
	
	rect.OffsetBy(0,25);
	rect.right=rect.left+36;
	fMeter=new GfxMeter(rect, meter);
	AddChild(fMeter);
}


//--
