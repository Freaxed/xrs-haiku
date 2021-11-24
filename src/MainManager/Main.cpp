#include "TheApp.h"
#include "stdio.h"
#include "Xed_Utils.h"

int
main()
{
	XUtils::CheckMimeType(NULL);
	
	TheApp app("application/x-vnd.xeD.XRS");
	app.Run();
	return 0;
}
