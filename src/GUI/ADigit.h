/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _ADigit_H
#define _ADigit_H

#include <Control.h>
#include <Handler.h>
#include <Message.h>
#include "Log.h"

class ADigit : public BControl
{
public:
	ADigit(BRect frame,BMessage *message,
		int32 minValue, int32 maxValue,
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW);


	void	SetMax(int32 value){ m_fMaxValue=value;}
	void 	UpdateValue(int32 newValue, bool invoke);
	int32	GetValue(){ return Value();};
	
	 void MouseDown(BPoint where);
	 void MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
	 void MouseUp(BPoint where);	
	 void MessageReceived(BMessage*);
	
	 void Draw(BRect updateRect);
	
	void	SetSens(int v){ sens=v;};
	
	
private:
	void 	SetValue(int32 newValue);
	void 	set_mouse(BPoint p);
	
	int32 	m_fMinValue; 
	int32	m_fMaxValue;

	const char * namex;
	bool m_bMark;
	
	bool track;
	
	BRect m_rectKnob;
	
	BPoint m_ptPrev;
	BPoint mouse_start;
	
	BBitmap *digit;

	int dig[3];
	char dix[4];
	int sens;

};

#endif 
