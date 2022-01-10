#include "App.h"
#include "MainWindow.h"
#include "Log.h"

App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	MainWindow *mainwin = new MainWindow();
	mainwin->Show();
}


int
main(void)
{
	Logger::SetLevel(LOG_LEVEL_TRACE);
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
