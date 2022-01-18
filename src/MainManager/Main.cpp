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
			printf("Log level set to OFF\n");
		break;
		case 'e':
			Logger::SetLevel(LOG_LEVEL_ERROR);
			printf("Log level set to ERROR\n");
		break;
		case 'i':
			Logger::SetLevel(LOG_LEVEL_INFO);
			printf("Log level set to INFO\n");
		break;
		case 'd':
			Logger::SetLevel(LOG_LEVEL_DEBUG);
			printf("Log level set to DEBUG\n");
		break;
		case 't':
			Logger::SetLevel(LOG_LEVEL_TRACE);
			printf("Log level set to TRACE\n");
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
		CheckLogLevel('i'); //INFO

	XUtils::CheckMimeType(NULL);
	
	TheApp app("application/x-vnd.xeD.XRS");
	app.Run();
	return 0;
}
