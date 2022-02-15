/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ValuableManager.h"


void	
ValuableManager::MessageReceived(BMessage* msg) {
	
	switch(msg->what) {
		case MSG_VALUABLE_CHANGED:
		{
			//Here we are locked
			BString vID;
			type_code typeFound = -1;

			if (msg->FindString(VAL_ID, &vID) == B_OK ) {
			    	
				iterator iter = fRegisteredValuable.find(vID);
	    		if(iter == fRegisteredValuable.end()) {
	    			LogError("No vID [%s] found in registered list!",vID.String());
	    			return;
	    		}
	    		BMessage* valuable = iter->second->mLastMessage;

				SetValuableValue(msg, valuable);	    

				SpreadToValuableReceivers(iter->second->lViews, valuable);  		
	    				
			} else {
				LogDebug("Check [%s] to %d ", vID.String(), typeFound);
			}
		}
		break;
		//Single int32 value coming from haiku widgets directly..
		case MSG_BEVALUE32_TO_VALUABLE:
			BString vID;
			type_code typeFound = -1;

			if (msg->FindString(VAL_ID, &vID) == B_OK ) {
			    	
				iterator iter = fRegisteredValuable.find(vID);
	    		if(iter == fRegisteredValuable.end()) {
	    			LogError("No vID [%s] found in registered list!",vID.String());
	    			return;
	    		}
	    		BMessage* valuable = iter->second->mLastMessage;
				int32 bevalue = 0;
				if (msg->FindInt32("be:value", 0, &bevalue) == B_OK) {
					valuable->ReplaceInt32(VAL_DATA_KEY, 0, bevalue);
				}
	    		SpreadToValuableReceivers(iter->second->lViews, valuable);
	    				
			} else {
				LogDebug("Check [%s] to %d ", vID.String(), typeFound);
			}
		break;
	
	}
	
	BLooper::MessageReceived(msg);
}	

void ValuableManager::SetValuableValue(BMessage* input, BMessage* output) {

		type_code typeFound = -1;
		int32 countFound = 1;
		if(input->GetInfo(VAL_DATA_KEY,&typeFound, &countFound) != B_OK)
				return;	
			
			void* ptr = NULL;
			ssize_t dataSize = 0;
			if (input->FindData(VAL_DATA_KEY, typeFound, 0, (const void **)&ptr, &dataSize) == B_OK) {
				output->ReplaceData(VAL_DATA_KEY, typeFound, 0, ptr, dataSize);
			}
			if (countFound > 1)
			{
			  if (input->FindData(VAL_DATA_KEY, typeFound, 1, (const void **)&ptr, &dataSize) == B_OK) {
			    	output->ReplaceData(VAL_DATA_KEY, typeFound, 1, ptr, dataSize);
			  }
			}
		}

void	
ValuableManager::SpreadToValuableReceivers(BList& lViews, BMessage* valuable)
{
		//spread the update to all the ValuableReceiver!
		for (int i=0;i<lViews.CountItems();i++) {
			BMessenger(((BHandler*)lViews.ItemAt(i))).SendMessage(valuable);
		} 
}

bool	
ValuableManager::UnregisterValuable(ValuableID vID) {
	if (Lock()){
		iterator iter = fRegisteredValuable.find(vID);
   		
   		if(iter == fRegisteredValuable.end())
   		{
   			LogError("UnregisterValuable: No Valuable registered [%s]", vID.String());
   			Unlock();
   			return false;
   		}
   		ValuableInfo*	vInfo = iter->second;
   		fRegisteredValuable.erase(iter);
   		delete vInfo;
   		iter = fRegisteredValuable.find(vID);
   		if(iter == fRegisteredValuable.end()) {
   			LogDebug("UnregisterValuable:  Valuable unregistered [%s]", vID.String());
   			if (fMonitor)
	    		fMonitor->UnregisterValuable(vID);
   		}
		Unlock();
		return true;
	}
	return false;
}	

bool	
ValuableManager::RegisterValuableReceiver(ValuableID vID, BHandler* receiver, bool doUpdate){
	if (Lock()) {
		iterator iter = fRegisteredValuable.find(vID);
			
		if(iter == fRegisteredValuable.end())
		{
			LogDebug("RegisterValuableReceiver: No Valuable registered [%s]", vID.String());
			Unlock();
			return false;
		}
		
		if (iter->second->lViews.HasItem((void*)receiver))
		{
			LogDebug("RegisterValuableReceiver: ValuableReceiver already registered [%s]", vID.String());
			Unlock();
			return false;
		}
		
		iter->second->lViews.AddItem((void*)receiver);
		LogDebug("RegisterValuableReceiver: ValuableReceiver registered [%s]. Views: %d", vID.String(), iter->second->lViews.CountItems());
		
		if (doUpdate) {
			BMessenger(receiver).SendMessage(iter->second->mLastMessage);
		}
					
		Unlock();
		return true;
	}	
	return false;
}

bool	
ValuableManager::GetInternalLastMessage(ValuableID vID, BMessage& fValue){
	if (Lock()) {
		iterator iter = fRegisteredValuable.find(vID);
			
		if(iter == fRegisteredValuable.end())
		{
			LogDebug("GetInternalLastMessage: No Valuable registered [%s]", vID.String());
			Unlock();
			return false;
		}
				
		fValue = *iter->second->mLastMessage;
					
		Unlock();
		return true;
	}	
	return false;
}


bool	
ValuableManager::UnregisterValuableReceiver(ValuableID vID, BHandler* receiver) {
	if (Lock()){
		iterator iter = fRegisteredValuable.find(vID);
		if(iter == fRegisteredValuable.end())
		{
			LogDebug("UnregisterValuableReceiver: No Valuable registered [%s]", vID.String());
			Unlock();
			return false;
		}
		if (iter->second->lViews.HasItem((void*)receiver) == false)
		{
			LogDebug("UnregisterValuableReceiver: ValuableReceiver doesn't have registered [%s]", vID.String());
			Unlock();
			return false;
		}
		
		iter->second->lViews.RemoveItem(receiver);
		LogDebug("UnregisterValuableReceiver: ValuableReceiver unregistered [%s]", vID.String());

		Unlock();
		return true;
	}
	return false;

}

void	
ValuableManager::AttachMonitorValuableManager(MonitorValuableManager* monitor) {
	if (Lock()){
	fMonitor = monitor;
	iterator iter = fRegisteredValuable.begin();
		while(iter != fRegisteredValuable.end()){
			ValuableID	id = iter->first;
			LogDebug("AttachMonitorValuableManager [%s]", id.String());
			monitor->RegisterValuable(id);
			iter++;
		}
	Unlock();
	}
}
void
ValuableManager::UpdateValue(BMessage& fValue) {
	PostMessage(&fValue);
}

void
ValuableManager::Dump() {
	//WARN: this could crash if the BHandler are already destroyed!
	if (Lock()){
		HDDEBUG("---------------------------------------------");
		iterator iter = fRegisteredValuable.begin();
		while(iter != fRegisteredValuable.end()){
			BList& vlist  = iter->second->lViews;
			BMessage* msg = iter->second->mLastMessage;
			ValuableID	id = iter->first;
			LogDebug("> ValuableID [%s] : ", id.String());
			if (Logger::IsDebugEnabled()) msg->PrintToStream();
			for (int i=0; i<vlist.CountItems(); i++)
			{
				BHandler* view = (BHandler*)vlist.ItemAt(i);
				LogDebug("-----> ValuableReceiver [%s]", view->Name());
			}
			iter++;
		}
		HDDEBUG("---------------------------------------------");
		Unlock();
  } 
}

