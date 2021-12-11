#ifndef VALUABLEMANAGER_H
#define VALUABLEMANAGER_H

#include "Valuable.h"
#include "Log.h"
#include <Looper.h>
#include <Messenger.h>
#include <map>
#include "MonitorValuableManager.h"


class ValuableReceiver  {
		public:
			ValuableReceiver(){}
			virtual BHandler* GetHandler() = 0;	
};


class ValuableManager : public BLooper {

	private:
				ValuableManager():fMonitor(NULL) {	}
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
		
		bool	RegisterValuableReceiver(ValuableID vID, ValuableReceiver* receiver, bool doUpdate = true);
		
		bool	UnregisterValuableReceiver(ValuableID vID, ValuableReceiver* receiver);
		
		template<class TYPE>
		bool	RegisterValuable(ValuableID vID, TYPE initialValue);
		
		template<class TYPE>
		bool	RegisterValuable(ValuableID vID, TYPE initialValue1, TYPE initialValue2);
		
		bool	UnregisterValuable(ValuableID vID);
		
		
		void	MessageReceived(BMessage* msg);
		
		void	Dump();

	
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

	    LogDebug("Registered Valuable [%s].", vID.String());
	    
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

	    LogDebug("Registered Valuable [%s].", vID.String());
	
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
