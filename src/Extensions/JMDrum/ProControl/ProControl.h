#ifndef ProControl_h_
#define ProControl_h_

#include <Control.h>

class RoundSlider;

class ProControl : public BControl {

	public:
				ProControl(BPoint where,int32);
	virtual	void Draw(BRect);
	virtual	void	SetLabel(const char*);
	virtual	void	AttachedToWindow();

	private:
			RoundSlider*	knob;
			BRect	r_knob;
			BRect	r_name;
			BRect	r_label;

};
#endif