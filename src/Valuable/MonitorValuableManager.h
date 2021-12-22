#ifndef MONITOR_VALUABLE_MANAGER_H
#define MONITOR_VALUABLE_MANAGER_H

class MonitorValuableManager {
public:

	virtual void RegisterValuable(ValuableID)    = 0;
	virtual void UnregisterValuable(ValuableID)  = 0;

};

#endif //MONITOR_VALUABLE_MANAGER_H
