#ifndef _BasicValuableView_H_
#define _BasicValuableView_H_

#include "ValuableView.h"


class BasicValuableView : public ValuableView {
	
	public:
			 BasicValuableView(int channel, BString name, BHandler* dest)
			 : ValuableView(channel, name){ fHandler = dest; };
			
		
	
	protected:
	friend class ValuableManager;
			BHandler*	Handler() { return fHandler; }
	
			int	fChannel;
			BHandler*	fHandler;
};
#endif
