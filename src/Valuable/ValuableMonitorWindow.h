#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "ValuableManager.h"
#include "MonitorValuableManager.h"
#include <map>
class BSlider;
class BStringItem;
class BScrollView;
class BListView;

class ValuableMonitorWindow : public BWindow , public MonitorValuableManager
{
public:
						ValuableMonitorWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);

			
protected:
friend MonitorValuableManager;			
			void RegisterValuable(ValuableID);
	        void UnregisterValuable(ValuableID);
			
private:

	BString GetValuableString(ValuableID vID, BMessage* input);

	std::map<ValuableID, BStringItem*>	mValuableLabel;
	typedef std::map<ValuableID, BStringItem*>::iterator Iterator;
	BSlider* volume;
	
//	BScrollView*	fScrollView;
	BListView*	    fValuePanel;
};

#endif
