#include "MainWindow.h"

#include <Application.h>
#include "StereoVolumesView.h"

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"Main Window",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	StereoVolumesView* svv = new StereoVolumesView("xrs.gain", "xrs.pan");
	AddChild(svv);
	svv->ResizeToPreferred();
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
