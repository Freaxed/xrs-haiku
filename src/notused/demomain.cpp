#include <Application.h>
#include <stdio.h>

#include "MBWindow.h"
#include "WindowManager.h"
#include "Xed_Utils.h"
#include "Configurator.h"
#include "AboutBox.h"
#include "ValuableManager.h"

int
main()
{

	//XUtils::ResetList(NULL); //done in AboutBox! (to use an offline view)
	
	BApplication app("application/x-vnd.xeD.XRS");
	
	Config()->cf_Init("XRSConfig");
		
	MBWindow* window = new MBWindow();
	
	WindowManager::Get()->Show(window);	
	
	app.Run();	
	
	return 0;
}
