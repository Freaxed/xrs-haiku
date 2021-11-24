#ifndef _ValuablePlayer_H_
#define _ValuablePlayer_H_

#include	"Looper.h"
#include "ValuableView.h"
#include "ValuableController.h"

class ValuablePlayer : public ValuableController //, public BLooper
{
public:
						ValuablePlayer();
						void play();
//	virtual	void		MessageReceived(BMessage *message);
//	virtual	void		AttachedToWindow();
//	virtual	BHandler*	Handler() { return this;};
};

#endif
