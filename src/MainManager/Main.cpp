#include "TheApp.h"
#include "stdio.h"
#include "Xed_Utils.h"
#include "Log.h"

int
main()
{
	Logger::SetLevel(LOG_LEVEL_TRACE);
	XUtils::CheckMimeType(NULL);
	
	TheApp app("application/x-vnd.xeD.XRS");
	app.Run();
	return 0;
}
