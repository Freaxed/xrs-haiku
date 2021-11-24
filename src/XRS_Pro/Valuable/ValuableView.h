#ifndef _ValuableView_H_
#define _ValuableView_H_

#include <Handler.h>

#include "Valuable.h"
//#include "ValuableManager.h"

#define		MSG_VALUABLE_CHANGED	'valc'

class ValuableView {
	
	public:
			 ValuableView(int channel){ fChannel=channel; };
		//	~ValuableView();
			
	public:
			int		GetChannel(){ return fChannel; };
			bool	RegisterForValuable(ValuableID);	
			//void	ValueChanged(Valuable*);
	
	//protected:
	//friend ValuableManager;
			virtual	BHandler*	Handler() = 0;
	
			int	fChannel;
};
#endif
