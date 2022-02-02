/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */
/***************************************
/
/	SimpleJuice beta 02
/
/   Something like a DrumMachine.
/	Original idea by Andrea Anzani
/	
/	I hope to find someone that can help me in get
/	this app better!
/	Please write two lines to me.
/
/	andrea.anzani@lib.disco.unimib.it
/
*****************************************/

#include <View.h>
#include <Synth.h>

class SampleView : public BView
{
	public:
				SampleView(BRect);
	
	 	void Draw(BRect);
			void Init(int chan,short *data,int32,bool);
	 	void AttachedToWindow();
			void	SetReversed(bool v){rev=v;Invalidate();}
			void	SetAmp(float v){amp=v;Invalidate();}
			
	private:
		int16	left;
		int16	right;
		float		left_p,right_p;
		int32	sampleCount;
		
		short		*buffer;
		bool 		rev;
		float 	zero;
		float 	incy;
		int 		incx;
		float		amp;
		int 		channels;
		
};

