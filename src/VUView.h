/*
 * Copyright 2005, Jérôme Duval. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Inspired by SoundCapture from Be newsletter (Media Kit Basics: Consumers and Producers)
 */

#ifndef VUVIEW_H
#define VUVIEW_H

#include <Bitmap.h>
#include <View.h>

#include "ValuableManager.h"


class VUView : public BView , public ValuableReceiver
{
public:
	VUView(BRect rect, ValuableID id);
	~VUView();
	void AttachedToWindow();
	void DetachedFromWindow();
	void Draw(BRect updateRect);
	void MessageReceived(BMessage*);
	BHandler*	GetHandler(){ return this; };
	void ComputeNextLevel(void *data, size_t size);
	
	int32 GetValue(int32 channel){ return channel == 0 ? value_l : value_r ;}
		
private:
	void Run();
	void Quit();
	static int32 RenderLaunch(void *data);
	void RenderLoop();
	
	thread_id fThreadId;
	BBitmap *fBitmap;
	BView *fBitmapView;
	bool fQuitting;
	int32 fLevelCount;
	int32 *fCurrentLevels;
	int32 fChannels;
	ValuableID	vID;
	int32 value_r;
	int32 value_l;
};

#endif	/* VUVIEW_H */
