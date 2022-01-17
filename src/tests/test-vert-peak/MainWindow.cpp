#include "MainWindow.h"

#include <Application.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>
#include "VertPeakView.h"
#include "PeakView.h"
#include <Button.h>
#include <MessageRunner.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Box.h>

VertPeakView* peakV;
PeakView* peakH;

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"Main Window",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	BGroupLayout* group = BLayoutBuilder::Group<>(B_HORIZONTAL);
	SetLayout(group);
	group->AddView(peakV = new VertPeakView("_peak_", false, false));
	group->AddView(peakH = new     PeakView("_peak_", false, false));
	peakV->SetPeakNotificationWhat('yyyy');
	peakH->SetPeakNotificationWhat('yyyy');

	
	BMessage message('xxxx');
	new BMessageRunner(BMessenger(this), &message, 20000);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case 'xxxx': {
			BMessage* x = new BMessage('yyyy');
			x->AddFloat("max", (float)rand()/(float)RAND_MAX); //left?
			x->AddFloat("max", (float)rand()/(float)RAND_MAX); //right?
			PostMessage(x, peakV);//->Invalidate();
			PostMessage(x, peakH);//->Invalidate();
		}
		break;
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
