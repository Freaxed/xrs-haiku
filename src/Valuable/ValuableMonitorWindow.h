#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "ValuableManager.h"
#include "MonitorValuableManager.h"
#include <map>
class BSlider;
class BTextView;

class ValuableMonitorWindow : public BWindow , public ValuableReceiver, public MonitorValuableManager
{
public:
						ValuableMonitorWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
			BHandler*	GetHandler() { return this; }
			
protected:
friend MonitorValuableManager;			
			void RegisterValuable(ValuableID);
	        void UnregisterValuable(ValuableID);
			
private:

	BString GetValuableString(ValuableID vID, BMessage* input);

	std::map<ValuableID, BTextView*>	mValuableLabel;
	typedef std::map<ValuableID, BTextView*>::iterator Iterator;
	BSlider* volume;
};

#endif
