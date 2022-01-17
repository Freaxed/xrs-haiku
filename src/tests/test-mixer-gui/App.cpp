#include "App.h"
#include "MixerWindow.h"


App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	MixerWindow *mainwin = MixerWindow::Get();
	mainwin->Show();
}


int
main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
