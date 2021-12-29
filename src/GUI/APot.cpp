/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "APot.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <WindowScreen.h>
#include <Application.h>
#include <Window.h>
#include <SupportDefs.h>

#include "PotViewer.h"

static float rads(float degrees);

#define MSG 'sett'

#define LZ	10


APot::APot(BRect frame, const char *name,
	BMessage *message, BMessage *state,
	int32 minValue, int32 maxValue,BBitmap *p1,BBitmap *p2)
	: BControl(frame, name, NULL, message, B_NOT_RESIZABLE, B_FRAME_EVENTS | B_WILL_DRAW | B_NAVIGABLE)
{
	Init(message, state, minValue, maxValue, p1, p2);		
}

APot::APot(const char *name,
	BMessage *message, BMessage *state,
	int32 minValue, int32 maxValue,BBitmap *p1,BBitmap *p2)
	: BControl(name, "", message, B_FRAME_EVENTS | B_WILL_DRAW | B_NAVIGABLE)
{
	Init(message, state, minValue, maxValue, p1, p2);		
}

void	
APot::Init(BMessage *message, BMessage *state,
             int32 minValue, int32 maxValue,
             BBitmap *p1 = NULL ,BBitmap *p2 = NULL)
{
		SetEnabled(true);
		m_fMinValue = minValue;
		m_fMaxValue = maxValue;
		m_fMinAngle = -150; 
		m_fMaxAngle =  150; 
		m_fDragAngle = 0;
		m_SwitchStateMessage = state;
		distance = 0;
		m_PenSize = 1.0;
		m_ShowValue = true;
		
		SetValue(minValue);
	
		BRect b = Bounds();
		FrameResized(b.Width(), b.Height());
		
		m_ReleaseMessage.what = SB_MSG;
	
		pad=p1;
		padoff=p2;
		SetViewColor(B_TRANSPARENT_COLOR);
}

void	
APot::FrameResized(float newWidth, float newHeight)
{
	BRect b = Bounds();
	m_PenSize = floor(newWidth / 10.0f);
	m_rectKnob.Set(0, 0, newWidth - 2*m_PenSize, newHeight- 2*m_PenSize);
	m_rectKnob.OffsetBy(b.left + (b.Width() - m_rectKnob.Width())/2,b.top + (b.Height() - m_rectKnob.Height())/2);	
	Invalidate();
}
		             
		             
APot::~APot() {}

void 
APot::SetOn(bool val){
	SetEnabled(val);
	Invalidate();
}

bool
APot::IsOn(){
	return IsEnabled();
}

void APot::GetValueRange(int32 *min, int32 *max) const {
	*min = m_fMinValue; *max = m_fMaxValue;	
}

void APot::SetValueRange(int32 min, int32 max){
	assert(min != max);
	m_fMinValue = min; m_fMaxValue = max;
}

void APot::SetValue(int32 newValue)
{
	if (newValue < m_fMinValue) newValue = m_fMinValue;
	if (newValue > m_fMaxValue) newValue = m_fMaxValue;
	if (newValue != Value()) {
		BControl::SetValue(newValue);
		Invoke();
	}
}
void	
APot::UpdateValue(int32 newValue){
	if (newValue < m_fMinValue) newValue = m_fMinValue;
	if (newValue > m_fMaxValue) newValue = m_fMaxValue;
	if (newValue != Value()) {
		BControl::SetValue(newValue);
	}
}

float APot::Angle() const {
	return AngleForValue(Value());
}

float APot::SetAngle(float newDegrees){
	SetValue(ValueForAngle(newDegrees));
	return Angle() - newDegrees;
}

int32 APot::ValueForAngle(float degrees) const{
	assert(m_fMinAngle != m_fMaxAngle);
	return (int32) floor(
		(m_fMinValue + (m_fMaxValue - m_fMinValue) *
			(degrees - m_fMinAngle) / (m_fMaxAngle - m_fMinAngle))
		+ 0.5);	
}

float APot::AngleForValue(int32 value) const{
	assert(m_fMinValue != m_fMaxValue);
	return (m_fMinAngle +
		((m_fMaxAngle - m_fMinAngle) *
			(value - m_fMinValue)) / (m_fMaxValue - m_fMinValue));
}


void 
APot::AttachedToWindow(){

	if (Parent())
		SetViewColor(Parent()->ViewColor());
	BControl::AttachedToWindow();
}

void APot::Draw(BRect updateRect){
	SetDrawingMode(B_OP_ALPHA);
	SetPenSize(m_PenSize);
	
	if(!IsEnabled() && padoff!=NULL) 
		DrawBitmap(padoff);
	else 
		if(pad!=NULL) 
			DrawBitmap(pad);
	else
	{
		SetHighColor(ViewColor());
		FillRect(updateRect);
		if (IsTracking())
			SetHighColor(255,0,0);
		else
			SetHighColor(0,0,0);
		
		StrokeArc(Bounds().InsetByCopy(m_PenSize / 2.0f, m_PenSize / 2.0f ), -65, 305);
	}		
	
	if(IsTracking()){
		rgb_color yellow = {255,255,55};
		BRect b(Bounds());
		BeginLineArray(1);
		
		AddLine(BPoint(0,0),BPoint(LZ,0),yellow);
		AddLine(BPoint(b.right,b.bottom),BPoint(b.right,b.bottom-LZ),yellow);
		
		EndLineArray();
	
	}
	
	DrawKnob(updateRect);
}




void APot::DrawKnob(BRect updateRect)
{
	BRect calc = KnobRect();
	
	calc.InsetBy(-1,-1);
	
	if (updateRect.Intersects(calc)) {
	
		PushState();
					
		float r = KnobRadius() / 2 ;
		r--;
		BPoint o = KnobOrigin();
		
		BPoint p;
				
		p.x = o.x + r*sin(rads(Angle()));
		p.y = o.y - r*cos(rads(Angle()));
		
		SetHighColor(255,255,55);
		SetLineMode(B_ROUND_CAP,B_ROUND_JOIN);
		StrokeLine(o, p);
		
				
		PopState();
	}
}

void APot::MouseDown(BPoint where)
{
	uint32 buttons;
	GetMouse(&where, &buttons);
	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		if (Bounds().Contains(where)) {

			m_fDragAngle = Angle();
			SetTracking(true);
			SetMouseEventMask(B_POINTER_EVENTS,
				B_LOCK_WINDOW_FOCUS | B_NO_POINTER_HISTORY);
		}
		
		mouse_start = where;
		rel_mouse_start = where;
		ConvertToScreen(&mouse_start);
		be_app->HideCursor();
		
		if (m_ShowValue) {
			PotViewer::Get()->InitShow(this,distance);
			PotViewer::Get()->SetValue(Value());
		}
		
		Invalidate();
	}
	
	else
		if (buttons & B_SECONDARY_MOUSE_BUTTON) {
			if(m_SwitchStateMessage!=NULL){
				SetOn(!IsEnabled());		
				Window()->PostMessage(m_SwitchStateMessage,Parent());
			}
		}
}	

void APot::MouseMoved(BPoint where, uint32 /* transit */, const BMessage* /* dragDropMsg */)
{
	
	if(where == rel_mouse_start) return;
	 
	if (IsTracking()) {
	
		float incr = CalcAngleIncr(rel_mouse_start, where);
		incr += SetAngle(m_fDragAngle);
		if (incr != 0){
			DrawKnob(Bounds());
			if (m_ShowValue) PotViewer::Get()->SetValue(Value());
		}
		set_mouse_position((long)mouse_start.x,(long)mouse_start.y);
		
		
	}
}

void APot::MouseUp(BPoint where)
{
	if (IsTracking()) {
		if (rel_mouse_start != where) {
			float incr = CalcAngleIncr(rel_mouse_start, where);
			incr += SetAngle(m_fDragAngle);
			
			if (incr != 0)
				DrawKnob(Bounds());
		}
		
		SetTracking(false);

		set_mouse_position((long)mouse_start.x,(long)mouse_start.y);
		be_app->ShowCursor();
		if (m_ShowValue) PotViewer::Get()->InitHide();
		Invalidate();
		
	}
	
	Window()->PostMessage(&m_ReleaseMessage);
	
	
}

void
APot::SetReleaseMessage(BMessage* release_message){
	m_ReleaseMessage = (*release_message);
}

float APot::CalcAngleIncr(BPoint prev, BPoint cur){
	float incr = GetDragAngle(prev, cur);
	m_fDragAngle += incr;
	if (m_fDragAngle > m_fMaxAngle) {
		m_fDragAngle = m_fMaxAngle;
	} else if (m_fDragAngle < m_fMinAngle) {
		m_fDragAngle = m_fMinAngle;
	} 
	
	return m_fDragAngle;
}

BRect APot::KnobRect() const{
	return m_rectKnob;
}

BPoint APot::KnobOrigin() const{
	BPoint o(m_rectKnob.left, m_rectKnob.top);
	o.x += m_rectKnob.Width() / 2;
	o.y += m_rectKnob.Height() / 2;
	return o;	
}

float APot::KnobRadius() const {
	return (m_rectKnob.Width() );
}

float APot::GetDragAngle(BPoint prev, BPoint cur){
	return -2*(cur.y - prev.y);
}



// Utility function -- convert degrees to radians
static float rads(float degrees){
	return (float)(degrees*M_PI/180);
}
