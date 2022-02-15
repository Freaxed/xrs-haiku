#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "App.h"

class MainWindow : public BWindow
{
public:
				MainWindow(App* a);

	// We are implementing the virtual BWindow method MessageReceived so
	// that we can do something with the message that the button sends.
	void		MessageReceived(BMessage *msg);
	
	
	void		Stop();

private:
	// This property will hold the number of
	// times the button has been clicked.
	int32	fCount;
	App*	app;
};

#endif
