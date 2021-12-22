#include "TheApp.h"
#include "stdio.h"
#include "Xed_Utils.h"
#include "Log.h"

/*
	LOG_LEVEL_OFF		= 1,
	LOG_LEVEL_ERROR		= 2,
	LOG_LEVEL_INFO		= 3,
	LOG_LEVEL_DEBUG		= 4,
	LOG_LEVEL_TRACE		= 5
*/
void CheckLogLevel(char level) {
	
	switch(level){
		case 'o':
			Logger::SetLevel(LOG_LEVEL_OFF);
		break;
		case 'e':
			Logger::SetLevel(LOG_LEVEL_ERROR);
		break;
		case 'i':
			Logger::SetLevel(LOG_LEVEL_INFO);
		break;
		case 'd':
			Logger::SetLevel(LOG_LEVEL_DEBUG);
		break;
		case 't':
			Logger::SetLevel(LOG_LEVEL_TRACE);
		break;
		default:
			LogFatal("Invalid log level, valid levels are: o, e, i, d, t");
		break;
	}

}


int
main(int argc, const char** argv)
{
	if (argc > 1) 
		CheckLogLevel(argv[1][0]);
	else
		Logger::SetLevel(LOG_LEVEL_TRACE);

	XUtils::CheckMimeType(NULL);
	
	TheApp app("application/x-vnd.xeD.XRS");
	app.Run();
	return 0;
}
