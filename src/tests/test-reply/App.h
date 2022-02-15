#ifndef APP_H
#define APP_H

#include <Application.h>
#include <Looper.h>

class App : public BApplication
{
public:
	App(void);
	void Stop();
	
	BLooper*	looper;
};

#endif
