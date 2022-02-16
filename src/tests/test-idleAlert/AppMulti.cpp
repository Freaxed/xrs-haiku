#include "AppMulti.h"
#include <Alert.h>
#include <Button.h>
#include "Activity.h"
#include <GroupLayout.h>
#include <assert.h>
#include <StringView.h>
#include <stdio.h>
#include "Xed_Utils.h"


App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	BAlert*	tokern = XUtils::ShowIdleAlert("This is a long text\n Yes, even multiline!");
	sleep(3);
	

	BAlert*	second = XUtils::ShowIdleAlert("sdas");
	sleep(3);
	XUtils::HideIdleAlert(second);
	
	sleep(3);
	XUtils::HideIdleAlert(tokern);
	
	be_app->PostMessage(B_QUIT_REQUESTED);
}


int
main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
