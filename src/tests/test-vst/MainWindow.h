#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "VSTHost.h"

class MainWindow : public BWindow
{
public:
						MainWindow(VSTPlugin*);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
private:
};

#endif
