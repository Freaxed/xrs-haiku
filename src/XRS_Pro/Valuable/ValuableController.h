#ifndef _ValuableController_H_
#define _ValuableController_H_

#include "Valuable.h"

#include <Message.h>

#define	MSG_CONTROLLER_VALUE 'cval'

class ValuableController {

	public:	
			 ValuableController();
					
	public:
					
			void	SetValuableID(ValuableID id);
			void	SendValue(int id,float value,bigtime_t delay=0);
			
			void	SetSender(void* sender);
			void	SetDefaultChannel(int channel);
			
			BMessage*	CopyMessage();
	private:
	
			ValuableID	fValuableID;
			BMessage	information;
			BString		fValID;
};
#endif
