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
	BView(vframe,"x",0,B_WILL_DRAW|B_FRAME_EVENTS),
	mReversed(false),
	mBoost(1.0f),
	mViewBitmap(viewBitmap),
	mSample(NULL)
{

}
		
void
SampleView::Init(Sample* sample, bool reversed, float boost)
{
	mReversed = reversed;
	mBoost 	  = boost;
	mSample   = sample;
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
		DrawBitmap(mViewBitmap);
		
	
	if (mSample) {
		SetHighColor(238,235,227);
		float value;
		float zero = Bounds().Height() / 2.0f;
		int	  incx=(int)((float)mSample->fullframes/(float)Bounds().Width());
		BPoint previous(0, zero);
		for(int i=0; i < Bounds().IntegerWidth(); i++)
		{
			if(!mReversed)
				value = mSample->wave_data[0][i * incx] * mBoost;
			else
				value = mSample->wave_data[0][mSample->fullframes - ( i * incx)] * mBoost;

			BPoint next((float)i, (float)((zero) + (zero*value)));
		 	StrokeLine(previous, next);
 	     	previous = next;
	 	}		
	}  
}

