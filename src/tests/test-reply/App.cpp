#include "App.h"
#include "MainWindow.h"
#include <stdio.h>

App::App(void)
	:	BApplication("application/x-vnd.dw-ButtonDemo")
{
	MainWindow *mainwin = new MainWindow(this);
	mainwin->Show();
	looper = mainwin;
}
void
App::Stop()
{
	BMessage reply;
	printf("Before sending.. %d vs %d \n", find_thread(NULL), looper->Thread());
	if (find_thread(NULL) != looper->Thread())
		BMessenger(looper).SendMessage('msgX', &reply);
	else
		BMessenger(looper).SendMessage('msgX');
	printf("OK\n");
}

int
main(void)
{
	App *app = new App();
	app->Stop();
	app->Run();
	delete app;
	return 0;
}
