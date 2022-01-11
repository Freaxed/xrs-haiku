#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "VSTHost.h"

class BMenuBar;
class BMenu;

class MainWindow : public BWindow
{
public:
						MainWindow(VSTPlugin*);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
			void SetControls(BView* conf, bool scr);
			void SetPrograms(const char *name,BMenu* prog);
			
private:
			BMenuBar*	bar;
};

#endif
