#include "App.h"
#include "ValuableMonitorWindow.h"


App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	BWindow* x  = new BWindow(BRect(0,0,500,400),"Main Window",B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS);
	x->Show();

	ValuableMonitorWindow *mainwin = new ValuableMonitorWindow();
	ValuableManager::Get()->AttachMonitorValuableManager(mainwin);
	mainwin->Show();
}
