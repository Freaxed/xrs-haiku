#include "MainWindow.h"

#include <Application.h>
#include "SampleView.h"
#include "Sample.h"
#include <math.h>
#include <Bitmap.h>

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"Main Window",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	Sample*	sample = new Sample();
	
	sample->CreateBuffer(1024, 1);
	
	float angle = 0.0;
	for(int i=0;i<1024;i++) {
		sample->wave_data[0][i] = sin(angle);
		angle += (2.0f*M_PI)/1024.0f;
	}

	SampleView* sw;
	AddChild(sw = new SampleView(BRect(0,0,300,200), NULL));
	sw->SetViewColor(0,0,160);
	sw->Init(sample, false, 1.0f);
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
