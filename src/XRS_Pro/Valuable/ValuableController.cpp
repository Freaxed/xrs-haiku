#include "ValuableController.h"
#include "ValuableManager.h"
#include <Messenger.h>
#include <MessageRunner.h>

ValuableController::ValuableController(){ 
	
	fValID = "null";
	information.what = MSG_CONTROLLER_VALUE;
	information.AddString("valuable:id",fValID);
	information.AddFloat("valuable:value",0.0);	
	information.AddInt16("valuable:value:id",0);	
	information.AddPointer("valuable:controller",NULL);
}

void	
ValuableController::SetSender(void* sender) {
	information.ReplacePointer("valuable:controller",sender);
}

void	
ValuableController::SendValue(int id,float value,bigtime_t delay) {
	
	if(fValID.Compare("null") == 0) return;
	
	information.ReplaceFloat("valuable:value",value);
	information.ReplaceInt16("valuable:value:id",id);
	if(delay==0)
	 ValuableManager::Get()->messenger->SendMessage(&information);
	else 
	{
		//TODO CHECK THIS
		BMessageRunner::StartSending(*ValuableManager::Get()->messenger, &information, delay, 1);
	}
}

void	
ValuableController::SetDefaultChannel(int channel) {
	if(fValID.Compare("null") == 0) return;
	information.ReplaceInt16("valuable:value:id",channel);
}


void	
ValuableController::SetValuableID(ValuableID id) { 
	fValID=id;
	information.ReplaceString("valuable:id",fValID);
};

BMessage*	
ValuableController::CopyMessage(){
	return new BMessage(information);
}
