#ifndef _ValuableView_H_
#define _ValuableView_H_

#include <Handler.h>

#include "Valuable.h"
#include <String.h>

#define		MSG_VALUABLE_CHANGED	'valc'

class ValuableView {
	
	public:
			 ValuableView(int32 channel, BString name){ 
			 	fChannel = channel;
			 	fName = name;
			 };
			
	public:
			int32	GetChannel(){ return fChannel; };
			bool	RegisterForValuable(ValuableID);	
			
			const BString	GetViewName() { return fName; };

			virtual	BHandler*	Handler() = 0;
	
			int32	fChannel;
			BString  fName;
};
#endif
