#ifndef VALUABLEMANAGER_H
#define VALUABLEMANAGER_H

#include "Valuable.h"
#include "Log.h"
#include <Looper.h>
#include <Messenger.h>
#include <map>
#include "MonitorValuableManager.h"


class ValuableManager : public BLooper {

	private:
				ValuableManager():fMonitor(NULL) {	Run(); }
	public:
				static ValuableManager* Get() 
				{
				 	static	ValuableManager*	instance = NULL;
					if (instance == NULL) 
							instance = new ValuableManager();
					return instance;
				}
		
		template<class TYPE>
		void	UpdateValue(ValuableID vID, TYPE fValue);
		
		template<class TYPE>
		void	UpdateValue(ValuableID vID, TYPE fValue1, TYPE fValue2);

		// don't use this function unless you know what are you doing..
		// if an component needs to get data, just use RegisterValue
		bool	GetInternalLastMessage(ValuableID vID, BMessage& fValue);

		// don't use this function unless you know what are you doing..
		void	UpdateValue(BMessage& fValue);
		
		bool	RegisterValuableReceiver(ValuableID vID, BHandler* receiver, bool doUpdate = true);
		
		bool	UnregisterValuableReceiver(ValuableID vID, BHandler* receiver);
		
		template<class TYPE>
		bool	RegisterValuable(ValuableID vID, TYPE initialValue);
		
		template<class TYPE>
		bool	RegisterValuable(ValuableID vID, TYPE initialValue1, TYPE initialValue2);
		
		bool	UnregisterValuable(ValuableID vID);
		
		
		void	MessageReceived(BMessage* msg);
		
		void	Dump(); //WARN: this could crash if the BHandler are already destroyed!

	
		void	AttachMonitorValuableManager(MonitorValuableManager*);
private:		
		void SetValuableValue(BMessage* input, BMessage* output);	
		
		void	SpreadToValuableReceivers(BList& lViews, BMessage* valuable);	

		typedef BList ValuableViewList;
		struct ValuableInfo {
			BMessage*		 mLastMessage;
			ValuableViewList lViews;
		};
		
		typedef std::map<ValuableID, ValuableInfo*> 	vMap;
		
		vMap	fRegisteredValuable;
		typedef vMap::iterator	iterator;
		
		MonitorValuableManager*	fMonitor;
};


template<class TYPE>	
bool ValuableManager::RegisterValuable(ValuableID vID, TYPE initialValue)
{
	if (Lock()) {
	
	    iterator iter = fRegisteredValuable.find(vID);
	    		
	    if(iter != fRegisteredValuable.end())
	    {
	    	LogDebug("RegisterValuable: Valuable already registered [%s]", vID.String());
	    	Unlock();
	    	return false;
	    }

	    ValuableInfo*	vInfo = new ValuableInfo();
	    vInfo->mLastMessage = new Valuable<TYPE>(vID, initialValue);
	    fRegisteredValuable[vID] = vInfo;

	    LogDebug("Registered Valuable [%s]. Views: %d", vID.String(), vInfo->lViews.CountItems());
	    
	    if (fMonitor)
	    	fMonitor->RegisterValuable(vID);
	
	    Unlock();
		return true;
    }
    return false;
}


template<class TYPE>	
bool ValuableManager::RegisterValuable(ValuableID vID, TYPE initialValue1, TYPE initialValue2)
{
	if (Lock()) {
	
	    iterator iter = fRegisteredValuable.find(vID);
	    		
	    if(iter != fRegisteredValuable.end())
	    {
	    	LogDebug("RegisterValuable: Valuable already registered [%s]", vID.String());
	    	Unlock();
	    	return false;
	    }

	    ValuableInfo*	vInfo = new ValuableInfo();
	    vInfo->mLastMessage = new Valuable<TYPE>(vID, initialValue1, initialValue2);
	    fRegisteredValuable[vID] = vInfo;

	    LogDebug("Registered Valuable [%s]. Views: %d", vID.String(), vInfo->lViews.CountItems());
	
	    Unlock();
	    return true;
    }
    return false;
}



template<class TYPE>
void	
ValuableManager::UpdateValue(ValuableID vID, TYPE fValue) {
	
	Valuable<TYPE> bMessage(vID, fValue);
	PostMessage(&bMessage);
}

template<class TYPE>
void	
ValuableManager::UpdateValue(ValuableID vID, TYPE fValue1, TYPE fValue2) {
	
	Valuable<TYPE> bMessage(vID, fValue1, fValue2);
	PostMessage(&bMessage);
}



#endif
