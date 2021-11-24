#ifndef _ValuableRecorder_H_
#define _ValuableRecorder_H_

#include <interface/StringView.h>

#include "ValuableView.h"
#include "ValuableController.h"

//using namespace Z::Experimental::Interface;

class ValuableRecorder : public BStringView, public ValuableView
{
public:
						ValuableRecorder(BRect);
	virtual	void		MessageReceived(BMessage *message);
	virtual	void		AttachedToWindow();
	virtual	BHandler*	Handler() { return this;};



};

#endif
