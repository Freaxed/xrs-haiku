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


MixableBox::MixableBox(BPoint position, PMixable* node)
	:BBox(BRect(position,position + BPoint(WIDTH-1,HEIGHT-1)),
		  "a_mixable_box",B_FOLLOW_NONE,B_WILL_DRAW)
		  ,fMixable(node)
{
	ValuableID id = "<sistemami>"; //fMixable->GetName();
	
	if (id == "") 
		debugger("Register the Mixable before the MixableBox!");
	
	
	//Title
	fName=new FixedText(BRect(5,5,115,25),node->GetName().String());
	fName->SetAlignment(B_ALIGN_CENTER);
	AddChild(fName);
	
	BRect rect(Bounds());
	rect.InsetBy(5,5);
	rect.right = (rect.right-rect.left)/2;
	rect.top = 26;
		
	//Slider
	fSlider = new XChannelSlider(rect, "XChannelSlider", id, 0, NULL, 0, 127, B_VERTICAL);
	fSlider->SetLimitLabels(NULL,NULL);
	fSlider->SetHashMarks(B_HASH_MARKS_RIGHT);
	fSlider->SetHashMarkCount(16);
	AddChild(fSlider);

	//Pot
	rect.OffsetTo(rect.right + 1,rect.top);
	rect.bottom=rect.top+22;
	rect.right=rect.left+22;
	fPot = new XPot(rect, "XPot", id, 1, NULL, -64, +64, XUtils::GetBitmap(24), NULL);
	AddChild(fPot);
	
	//Meter
	id += ".meter";
	rect.OffsetBy(0,25);
	rect.right=rect.left+36;
	fMeter=new GfxMeter(rect, id);
	AddChild(fMeter);
}


//--
