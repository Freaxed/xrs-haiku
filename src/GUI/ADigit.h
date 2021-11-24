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

#include <View.h>
#include <Handler.h>
#include <Message.h>

class ADigit : public BView
{
public:
	ADigit(BRect frame,BMessage *message,BMessage *state,
		int32 minValue, int32 maxValue,
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW);


	void	SetMax(int32 value){ m_fMaxValue=value;}
	void 	SetValue(int32 newValue,bool invoke=true);
	int32	GetValue(){ return m_fCurValue;};
	
	 void MouseDown(BPoint where);
	 void MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
	 void MouseUp(BPoint where);	
	 void MessageReceived(BMessage*);
	
	 void Draw(BRect updateRect);
	
	void	SetSens(int v){ sens=v;};
	void	SetTarget(BHandler* t){target=t;}
	void	SetReleaseMessage(int32 val){ release->what=val;};
		
private:
	
	void	postMsg();
	void set_mouse(BPoint p);
	
	int32 m_fMinValue, m_fMaxValue,m_fCurValue;

	const char * namex;
	bool m_bMark;
	
	bool track;
	
	BRect m_rectKnob;
	
	BPoint m_ptPrev;
	BPoint mouse_start;
	BMessage	*rel_msg;
	BMessage	*switch_msg;
	
	BBitmap *digit;
	BHandler*	target;
	int dig[3];
	char dix[4];
	BMessage	*release;
	int sens;

};

#endif 
