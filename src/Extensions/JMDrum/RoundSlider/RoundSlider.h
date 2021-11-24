#ifndef ROUNDSLIDER_H
#define ROUNDSLIDER_H

#include <Control.h>

extern BBitmap *round_slider_bitmap;

class RoundSlider : public BControl
{
private:
	BBitmap *bitmap;
	//mouse tracking:
	int ydown;
	int buttondown;
	int ival;
	//
	float value;
public:
	RoundSlider(BRect frame,BMessage *message,uint32 resizingMode=B_FOLLOW_NONE);
	RoundSlider(BMessage *archive);
	~RoundSlider();
	static BArchivable *Instantiate(BMessage *archive);
	status_t Archive(BMessage *archive, bool deep = true) const;
	virtual void Draw(BRect where);
	virtual void MouseDown(BPoint point);
	virtual void MouseUp(BPoint point);
	virtual void MouseMoved(BPoint point, uint32 transit, const BMessage *message);
	//
	void SetValue(float val);
	float GetValue();
};

#endif
