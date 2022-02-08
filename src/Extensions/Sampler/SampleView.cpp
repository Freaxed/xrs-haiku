/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "SampleView.h"
#include <stdio.h>
#include <stdlib.h>
#include <Bitmap.h>
#include "Sample.h"


SampleView::SampleView(BRect vframe, BBitmap* viewBitmap):
	BView(vframe, "sampleView", 0, B_WILL_DRAW|B_FRAME_EVENTS),
	mReversed(false),
	mBoost(1.0f),
	mViewBitmap(viewBitmap),
	mSample(NULL)
{

}
		
void
SampleView::Init(Sample* sample, bool reversed, float boost)
{
	mSample   = sample;
	if (mSample) {
		mIncX = ((float)mSample->GetFullframes()/(float)Bounds().Width()) - 0.01; //Move on FrameResized!
		mReversed = reversed;
		mBoost 	  = boost;
		// mStart	  = (int)(0 * mIncX); //FIX
		// mEnd	  = (int)((sample->GetFullframes() - 1 ) * mIncX); //FIX
	}
	Invalidate();
}

void	
SampleView::SetReversed(bool reversed){
	mReversed = reversed;
	Invalidate();
}

void	
SampleView::SetBoost(float boost){
	mBoost = boost;
	Invalidate();
}

void
SampleView::Draw(BRect b)
{	
	if (mViewBitmap)
		DrawBitmapAsync(mViewBitmap);
		
	
	if (mSample) {
		SetHighColor(238,235,227);
		float value;
		float zero = Bounds().Height() / 2.0f;
		BPoint previous(0, zero);
		for(int i=b.left; i <= b.right; i++)
		{
			int index = (int)floor(((float)i * mIncX));

			if(!mReversed)
				value = mSample->wave_data[0][index] * mBoost;
			else
				value = mSample->wave_data[0][mSample->GetFullframes() - 1 - index] * mBoost;

			BPoint next((float)i, (float)((zero) + (zero*value)));
		 	StrokeLine(previous, next);
 	     	previous = next;
	 	}	
	 	// if (mStart >= b.left && mStart <= b.right)
	 	// {
	 	// 	SetHighColor(255, 0, 0);
	 	// 	StrokeLine(BPoint(mStart, b.top), BPoint(mStart, b.bottom));
	 	// }
	} 	
	Sync();
}

