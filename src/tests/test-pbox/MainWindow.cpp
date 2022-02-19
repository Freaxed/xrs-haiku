#include "MainWindow.h"

#include <Application.h>
#include "PBox.h"
#include <Message.h>


MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"Main Window",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	BMessage templ;
	BMessage values;
	BMessage s1;
	s1.AddString("name", "City");
	s1.AddString("description", "Select yout city");
	s1.AddInt32("type", B_STRING_TYPE);
	s1.AddString("valid_value", "Monza");
	s1.AddString("valid_value", "Catania");
	s1.AddString("valid_value", "Roma");
	s1.AddString("valid_value", "Firenze");

	BMessage s2;
	s2.AddString("name", "Street");
	s2.AddString("description", "Put your street");
	s2.AddInt32("type", B_STRING_TYPE);

	BMessage s3;
	s3.AddString("name", "Secret Name");
	s3.AddString("description", "Put your secret name");
	s3.AddInt32("type", B_STRING_TYPE);
	s3.AddBool("is_secret", true);

	BMessage s4;
	s4.AddString("name", "Long description");
	s4.AddString("description", "Put your long description here");
	s4.AddInt32("type", B_STRING_TYPE);
	s4.AddBool("multi_line", true);


	templ.AddMessage("setting", &s1);
	templ.AddMessage("setting", &s2);
	templ.AddMessage("setting", &s3);
	templ.AddMessage("setting", &s4);

	AddChild(new PBox(BRect(0,0, 400,300), templ, values, "Test"));
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
