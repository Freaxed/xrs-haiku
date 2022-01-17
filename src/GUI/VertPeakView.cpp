/*
 * Copyright (C) 2001-2010 Stephan Aßmus. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Copyright (C) 1998-1999 Be Incorporated. All rights reseved.
 * Distributed under the terms of the Be Sample Code license.
 */


#include "VertPeakView.h"

#include <new>
#include <stdio.h>
#include <string.h>

#include <Bitmap.h>
#include <ControlLook.h>
#include <Locale.h>
#include <MenuItem.h>
#include <Message.h>
#include <MessageRunner.h>
#include <Messenger.h>
#include <PopUpMenu.h>
#include <Window.h>

using std::nothrow;


enum {
	MSG_PULSE		= 'puls',
	MSG_LOCK_PEAKS	= 'lpks'
};


VertPeakView::VertPeakView(const char* name, bool useGlobalPulse, bool displayLabels)
	:
	BView(name, (useGlobalPulse ? B_PULSE_NEEDED : 0)
		| B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE),
	fUseGlobalPulse(useGlobalPulse),
	fDisplayLabels(false),//TOBEDEFINE displayLabels),
	fPeakLocked(false),

	fRefreshDelay(20000),
	fPulse(NULL),

	fChannelInfos(NULL),
	fChannelCount(0),
	fGotData(true),

	fBackBitmap(NULL),
	fPeakNotificationWhat(0)
{
	GetFontHeight(&fFontHeight);

	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetViewColor(B_TRANSPARENT_COLOR);

	SetChannelCount(2);
}


VertPeakView::~VertPeakView()
{
	delete fPulse;
	delete fBackBitmap;
	delete[] fChannelInfos;
}


void
VertPeakView::MessageReceived(BMessage* message)
{
	if (message->what == fPeakNotificationWhat) {
		float max;
		for (int32 i = 0; message->FindFloat("max", i, &max) == B_OK; i++)
			SetMax(max, i);
		fGotData = true;
		return;
	}

	switch (message->what) {
		case MSG_PULSE:
			Pulse();
			break;

		case MSG_LOCK_PEAKS:
			fPeakLocked = !fPeakLocked;
			break;

		default:
			BView::MessageReceived(message);
			break;
	}
}


void
VertPeakView::AttachedToWindow()
{
	if (!fUseGlobalPulse) {
		delete fPulse;
		BMessage message(MSG_PULSE);
		fPulse = new BMessageRunner(BMessenger(this), &message,
			fRefreshDelay);
	}
}


void
VertPeakView::DetachedFromWindow()
{
	delete fPulse;
	fPulse = NULL;
}


void
VertPeakView::MouseDown(BPoint where)
{
	int32 buttons;
	if (Window()->CurrentMessage()->FindInt32("buttons", &buttons) < B_OK)
		buttons = B_PRIMARY_MOUSE_BUTTON;

	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		// Reset the overshot flag and set the observed max to the current
		// value.
		for (uint32 i = 0; i < fChannelCount; i++) {
			fChannelInfos[i].last_overshot_time = -5000000;
			fChannelInfos[i].last_max = fChannelInfos[i].current_max;
		}
	} else if (buttons & B_TERTIARY_MOUSE_BUTTON) {
		// Toggle locking of the observed max value.
		fPeakLocked = !fPeakLocked;
	} else {
		// Display context menu
		BPopUpMenu* menu = new BPopUpMenu("peak context menu");
		BMenuItem* item = new BMenuItem("Lock Peaks",
			new BMessage(MSG_LOCK_PEAKS));
		item->SetMarked(fPeakLocked);
		menu->AddItem(item);
		menu->SetTargetForItems(this);

		menu->SetAsyncAutoDestruct(true);
		menu->SetFont(be_plain_font);
	
		where = ConvertToScreen(where);
		bool keepOpen = false; // ?
		if (keepOpen) {
			BRect mouseRect(where, where);
			mouseRect.InsetBy(-3.0, -3.0);
			where += BPoint(3.0, 3.0);
			menu->Go(where, true, false, mouseRect, true);
		} else {
			where += BPoint(3.0, 3.0);
			menu->Go(where, true, false, true);
		}
	}
}


void
VertPeakView::Draw(BRect updateRect)
{
	
	BRect r(_BackBitmapFrame());
	float width = r.Width();
	r.InsetBy(-2.0, -2.0);

	be_control_look->DrawTextControlBorder(this, r, updateRect, LowColor());

	// peak bitmap
	if (fBackBitmap)
		_DrawBitmap();

	// dB labels
	if (fDisplayLabels) {
		font_height fh;
		GetFontHeight(&fh);
		float y = Bounds().bottom;
		y -= fh.descent;
		DrawString("0", BPoint(4.0 + width - StringWidth("0"), y));
		DrawString("-6", BPoint(0.477 * width, y));
		DrawString("-12", BPoint(0.227 * width, y));
	}
}


void
VertPeakView::FrameResized(float width, float height)
{
	BRect bitmapFrame = _BackBitmapFrame();
	_ResizeBackBitmap(bitmapFrame.IntegerHeight() + 1, fChannelCount);
	_UpdateBackBitmap();
}


void
VertPeakView::Pulse()
{
//	if (!fGotData)
//		return;

	if (fBackBitmap == NULL)
		return;
		

	if (!fPeakLocked) {
		for (uint32 i = 0; i < fChannelCount; i++) {
			fChannelInfos[i].last_max *= 0.96f;
			if (fChannelInfos[i].current_max > fChannelInfos[i].last_max)
				fChannelInfos[i].last_max = fChannelInfos[i].current_max;
		}
	}
	_UpdateBackBitmap();

	for (uint32 i = 0; i < fChannelCount; i++)
		fChannelInfos[i].current_max = 0.0f;
	fGotData = false;

	_DrawBitmap();
	Flush();
}


BSize
VertPeakView::MinSize()
{
	float minWidth  =  2 * 8 - 1 + 4;
	float minHeight = 20 + 4;
	
	if (fDisplayLabels) {
		font_height fh;
		GetFontHeight(&fh);
		minWidth = max_c(60.0, minWidth);
		minHeight += ceilf(fh.ascent + fh.descent);
	}
	return BSize(minWidth, minHeight);
}


bool
VertPeakView::IsValid() const
{
	return fBackBitmap != NULL && fBackBitmap->IsValid()
		&& fChannelInfos != NULL;
}


void
VertPeakView::SetPeakRefreshDelay(bigtime_t delay)
{
	if (fRefreshDelay == delay)
		return;

	fRefreshDelay = delay;

	if (fPulse != NULL)
		fPulse->SetInterval(fRefreshDelay);
}


void
VertPeakView::SetPeakNotificationWhat(uint32 what)
{
	fPeakNotificationWhat = what;
}


void
VertPeakView::SetChannelCount(uint32 channelCount)
{
	if (channelCount == fChannelCount)
		return;

	delete[] fChannelInfos;
	fChannelInfos = new(std::nothrow) ChannelInfo[channelCount];
	if (fChannelInfos != NULL) {
		fChannelCount = channelCount;
		for (uint32 i = 0; i < fChannelCount; i++) {
			fChannelInfos[i].current_max 		= 0.0f;
			fChannelInfos[i].last_max 			= 0.0f;
			fChannelInfos[i].last_overshot_time = -5000000;
		}
		_ResizeBackBitmap(_BackBitmapFrame().IntegerHeight() + 1, fChannelCount);
	} else
		fChannelCount = 0;
}


void
VertPeakView::SetMax(float max, uint32 channel)
{
	if (channel >= fChannelCount)
		return;

	if (fChannelInfos[channel].current_max < max)
		fChannelInfos[channel].current_max = max;

	if (fChannelInfos[channel].current_max > 1.0)
		fChannelInfos[channel].last_overshot_time = system_time();
}


// #pragma mark -


BRect
VertPeakView::_BackBitmapFrame() const
{
	BRect frame = Bounds();
	frame.InsetBy(2, 2);
	if (fDisplayLabels)
		frame.bottom -= ceilf(fFontHeight.ascent + fFontHeight.descent);
	return frame;
}


void
VertPeakView::_ResizeBackBitmap(int32 height, int32 channels)
{
	if (fBackBitmap != NULL) {
		if (fBackBitmap->Bounds().IntegerHeight() + 1 == height
			&& fBackBitmap->Bounds().IntegerWidth() + 1 == channels) {
			return;
		}
	}
	if (channels <= 0)
		channels = 2;

	delete fBackBitmap;
	BRect bounds(0, 0, channels - 1, height - 1);
	fBackBitmap = new(std::nothrow) BBitmap(bounds, 0, B_RGB32);
	
	if (fBackBitmap == NULL || !fBackBitmap->IsValid()) {
		delete fBackBitmap;
		fBackBitmap = NULL;
		return;
	}
	memset(fBackBitmap->Bits(), 0, fBackBitmap->BitsLength());
	fGotData = true;
}

//void
//fill(uint8* span, uint32 height, float current, float peak,
//	bool overshot, uint8 blue)
//	{
//		for (uint32 x = 0; x < height; x +=2) {
//			span[0] = blue;
//			span[1] = 0;
//			span[2] = 255;
//			span += 16;
//		}
//	}
//
//
//void
//reversefill(uint8* span, uint32 height, float current, float peak,
//	bool overshot, uint8 blue)
//	{
//		for (uint32 x = 0; x < height; x +=2) {
//			span[0] = blue;
//			span[1] = 0;
//			span[2] = 255;
//			span -= 16;
//		}
//	}

void
VertPeakView::_UpdateBackBitmap()
{
	if (!fBackBitmap)
		return;

	uint32 height = fBackBitmap->Bounds().IntegerHeight() + 1;
	for (int32 i = fChannelCount - 1; i  >= 0 ; i--) {
		uint8* span = (uint8*)fBackBitmap->Bits() + fBackBitmap->BitsLength() - (4 * (fChannelCount - i));
		_RenderSpan(span, height, fChannelInfos[i].current_max,
			fChannelInfos[i].last_max,
			system_time() - fChannelInfos[i].last_overshot_time < 2000000);
	}
}

//void
//VertPeakView::_UpdateBackBitmap()
//{
//	if (!fBackBitmap)
//		return;
//
//	uint32 height = fBackBitmap->Bounds().IntegerHeight() + 1;
//	for (int32 i = fChannelCount - 1; i  >= 0 ; i--) {
//		uint8* span = (uint8*)fBackBitmap->Bits() + fBackBitmap->BitsLength() - (4 * (fChannelCount - i));
//		reversefill(span, height, 1.0f, 1.0f, true, 255*i);
//	}
//}


void
VertPeakView:: _RenderSpan(uint8* span, uint32 height, float current, float peak,
	bool overshot)
{
	uint8 emptyR = 15;
	uint8 emptyG = 36;
	uint8 emptyB = 16;

	uint8 fillR = 41;
	uint8 fillG = 120;
	uint8 fillB = 45;

	uint8 currentR = 45;
	uint8 currentG = 255;
	uint8 currentB = 45;

	uint8 lastR = 255;
	uint8 lastG = 229;
	uint8 lastB = 87;

	uint8 overR = 255;
	uint8 overG = 89;
	uint8 overB = 7;

	uint8 kFadeFactor = 100;

	uint32 evenH = height - height % 2;
	uint32 split = (uint32)(current * (evenH - 1) + 0.5);
	split += split & 1;
	uint32 last = (uint32)(peak * (evenH - 1) + 0.5);
	last += last & 1;
	uint32 over = overshot ? evenH : evenH + 1;
	over += over & 1;
	
	for (uint32 x = 0; x < height; x += 2) {
		uint8 fadedB = (uint8)(((int)span[0] * kFadeFactor) >> 8);
		uint8 fadedG = (uint8)(((int)span[1] * kFadeFactor) >> 8);
		uint8 fadedR = (uint8)(((int)span[2] * kFadeFactor) >> 8);
		if (x < split) {
			span[0] = max_c(fillB, fadedB);
			span[1] = max_c(fillG, fadedG);
			span[2] = max_c(fillR, fadedR);
		} else if (x == split) {
			span[0] = currentB;
			span[1] = currentG;
			span[2] = currentR;
		} else if (x > split) {
			span[0] = max_c(emptyB, fadedB);
			span[1] = max_c(emptyG, fadedG);
			span[2] = max_c(emptyR, fadedR);
		}
		if (x == last) {
			span[0] = lastB;
			span[1] = lastG;
			span[2] = lastR;
		}
		if (x == over) {
			span[0] = overB;
			span[1] = overG;
			span[2] = overR;
		}
		span -= 16;
	}
}


void
VertPeakView::_DrawBitmap()
{
	SetHighColor(0, 0, 0);
	BRect bitmapFrame = _BackBitmapFrame();
	BRect bitmapRect = fBackBitmap->Bounds();
	bitmapRect.right = bitmapRect.left;
	float channelW = (bitmapFrame.Width() + 1) / fChannelCount;
	for (uint32 i = 0; i < fChannelCount; i++) {
		BRect viewRect(bitmapFrame);
		viewRect.right = viewRect.left;
		viewRect.left  += floorf(i * channelW + 0.5);
		if (i < fChannelCount - 1) {
			viewRect.right += floorf((i + 1) * channelW + 0.5) - 2;
			StrokeLine(BPoint(viewRect.right  + 1, viewRect.top ), BPoint(viewRect.right + 1, viewRect.bottom ));
		} else {
			viewRect.right += floorf((i + 1) * channelW + 0.5) - 1;
		}
		DrawBitmapAsync(fBackBitmap, bitmapRect, viewRect);
		bitmapRect.OffsetBy(1, 0);
	}
}


