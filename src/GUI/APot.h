/*
 * 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _APot_h
#define _APot_h

#include <Control.h>

#define	SB_MSG	'sbmp'	//ReleaseMessage (mouse up)

class APot : public BControl
{
public:
	APot(BRect frame, const char *name,
		BMessage *valueChanging ,BMessage *switchState,
		int32 minValue, int32 maxValue,
		BBitmap *p1 = NULL ,BBitmap *p2 = NULL);
		
	APot(const char *name,
		BMessage *valueChanging ,BMessage *switchState,
		int32 minValue, int32 maxValue,
		BBitmap *p1 = NULL ,BBitmap *p2 = NULL);
		
	virtual ~APot();
		
	void 	GetValueRange(int32* min, int32* max) const;
	void 	SetValueRange(int32 min, int32 max);
		
	

			float	SetAngle(float newDegrees);
			float	Angle() const;
	virtual int32   ValueForAngle(float degrees) const;
	virtual float   AngleForValue(int32 value) const;
	
			void	GetPreferredSize(float* _width,
									float* _height) 
			{
						if (_width) *_width   = 40.0f;
						if (_height) *_height = 40.0f;
			}
			
			virtual	BSize				MinSize() { return BSize(40.0f, 40.0f); }
			virtual	BSize				MaxSize() { return BSize(40.0f, 40.0f); }
			void	FrameResized(float newWidth, float newHeight);
									
	
	void SetReleaseMessage(BMessage *);
	
	void	SetOn(bool);
	bool	IsOn();
	
	void	SetShowValue(bool show) { m_ShowValue = show;}

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
	
	void	SetValue(int32 newValue);
	
protected:
		void	UpdateValue(int32 newValue);
		
	
private:
	
		void	Init(BMessage *message, BMessage *state,
		             int32 minValue, int32 maxValue,
		             BBitmap *p1 ,BBitmap *p2);
	
	int32 		m_fMinValue, m_fMaxValue;
	float 		m_fMinAngle, m_fMaxAngle;
	float 		m_fDragAngle;
	
	float 		m_PenSize;
	
	BRect 		m_rectKnob;
	
	BPoint 		m_ptPrev;
	BPoint 		mouse_start;
	BPoint 		rel_mouse_start;
	
	BMessage	m_ReleaseMessage; 			//when mouseup.
	BMessage*	m_SwitchStateMessage;		//when right-click (if NULL function is disable)

	BBitmap*	pad;
	BBitmap*	padoff;
	bool		m_ShowValue;

};

#endif /* _pot_h */
