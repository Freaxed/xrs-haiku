#ifndef _AutoValuableController_H_
#define _AutoValuableController_H_

#include "ValuableController.h"

#define	MSG_CONTROLLER_VALUE 'cval'

class AutoValuableController : public ValuableController{

	public:	
			 AutoValuableController(ValuableID id,int channel):ValuableController(){
			 	
			 			SetValuableID(id);
						SendValue(channel id,float value,bigtime_t delay=0);
						SendSender(this);
			 }
					
	};
#endif
