/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */
/***************************************/


#include <View.h>

class Sample;

class SampleView : public BView
{
	public:
				SampleView(BRect, BBitmap* viewBitmap);
	
	 	void 	Draw(BRect);
		
		void 	Init(Sample* sample, bool reversed, float boost);
	 	
		void	SetReversed(bool reversed);
		void	SetBoost(float boost);
		
		
	private:

		bool 	 mReversed;
		float	 mBoost;
		BBitmap* mViewBitmap;
		Sample*	 mSample;
		
};

