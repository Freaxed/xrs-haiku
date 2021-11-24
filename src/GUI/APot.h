/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _APot_h
#define _APot_h

#include <Control.h>

#define	SB_MSG	'sbmp'

class APot : public BControl
{
public:
	APot(BRect frame, const char *name,
		BMessage *message,BMessage *satte,
		int32 minValue, int32 maxValue,
		BBitmap *p1 = NULL ,BBitmap *p2 = NULL);
		
	virtual ~APot();
		
	void 	GetValueRange(int32* min, int32* max) const;
	void 	SetValueRange(int32 min, int32 max);
		
	
	virtual	void	SetValue(int32 newValue);
			float	SetAngle(float newDegrees);
			float	Angle() const;
	virtual int32   ValueForAngle(float degrees) const;
	virtual float   AngleForValue(int32 value) const;
	
	void SetReleaseMessage(BMessage *);
	
	void	SetOn(bool);
	bool	IsOn();

	//events	
	
	virtual void AttachedToWindow();
	virtual void MouseDown(BPoint where);
	virtual void MouseMoved(BPoint where, uint32 code,const BMessage *dragDropMsg);
	virtual void MouseUp(BPoint where);	
	virtual void Draw(BRect updateRect);
	
	//draws
	
	
	void DrawKnob(BRect updateRect);

	float CalcAngleIncr(BPoint prev, BPoint cur);
	float GetDragAngle(BPoint prev, BPoint cur);
	
	BPoint	KnobOrigin() const;
	BRect 	KnobRect() const;
	float 	KnobRadius() const;
	
	float	distance;	//dirty thing
	
protected:
		void	UpdateValue(int32 newValue);
	
private:
	int32 m_fMinValue, m_fMaxValue;
	float m_fMinAngle, m_fMaxAngle;
	float m_fDragAngle;
			
	bool state_on;
	
	BRect 		m_rectKnob;
	
	BPoint 		m_ptPrev;
	BPoint 		mouse_start;
	BPoint 		rel_mouse_start;
	
	BMessage	rel_msg;
	BMessage	*switch_msg;

	BBitmap *tick;
	BBitmap *pad,*padoff;

};

#endif /* _pot_h */
