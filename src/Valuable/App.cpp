#include "App.h"
#include "ValuableMonitorWindow.h"


App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	ValuableMonitorWindow *mainwin = new ValuableMonitorWindow();
	ValuableManager::Get()->AttachMonitorValuableManager(mainwin);
	mainwin->Show();
}
