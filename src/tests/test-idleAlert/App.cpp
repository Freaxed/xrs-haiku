#include "App.h"
#include <Alert.h>
#include <Button.h>
#include "Activity.h"
#include <GroupLayout.h>
#include <assert.h>
#include <StringView.h>
#include <stdio.h>

void recursive(BLayout*	group, int level = 0) {
	for(int i=0;i< group->CountItems();i++) {
		
		BView* vv = group->ItemAt(i)->View();
		
		printf("lvl %d-%d %s\n",	level, i, vv ? vv->Name() : "");
		
		if (!vv)
		{
			BLayout* lay = dynamic_cast<BLayout*>(group->ItemAt(i));
			if (lay)
				recursive(lay, level+1);
		}
	}
	
	if (level == 1) {
		Activity *act = NULL;
		group->AddView(act = new Activity("_activity_")); //new BStringView("hey", "2"));
		act->Start();
		act->SetExplicitMinSize(BSize(10, 20));
		printf("here\n");
	}
}

void
SetIdleAlert(BAlert *al) {
	BGroupLayout* group = dynamic_cast<BGroupLayout*>(al->GetLayout());
	BLayout* lay = dynamic_cast<BLayout*>(group->ItemAt(group->CountItems()-1));	
	Activity *act = NULL;
	lay->AddView(act = new Activity("_activity_"));
	act->Start();
	act->SetExplicitMinSize(BSize(10, 20));
}

BAlert * ShowIdleAlert(const char* multilineText)
{
	BAlert * al = new BAlert("Wait", multilineText, "...");
	
	BGroupLayout* group = dynamic_cast<BGroupLayout*>(al->GetLayout());
	BLayout* lay = dynamic_cast<BLayout*>(group->ItemAt(group->CountItems()-1));	
	Activity *act = NULL;
	lay->AddView(act = new Activity("_activity_"));
	act->SetExplicitMinSize(BSize(10, 20));
	
	al->ButtonAt(0)->Hide();
	
	act->Start();
	al->Go(NULL);
	
	return al;
}

void HideIdleAlert(BAlert * alert)
{
	BAlert * al = (BAlert*)alert;
	if(al->Lock()) al->Quit();
}

void
SetIdleAlertX(BAlert *al)
{
	 BGroupLayout* group = dynamic_cast<BGroupLayout*>(al->GetLayout());
	 assert(group);
	 recursive(group);
}

App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	BAlert * tokern = ShowIdleAlert("This is a long text\n Yes, even multiline!");

	sleep(3);
	
	HideIdleAlert(tokern);
	
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
