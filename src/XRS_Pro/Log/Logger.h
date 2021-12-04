#ifndef _Logger_h_
#define _Logger_h_

//#include "BeDC.h"
#include <String.h>


enum loglevel {

	LOG_NONE  = 0x00000001,
	LOG_INFO  = 0x00000002,
	LOG_WARN  = 0x00000004,
	LOG_FATAL = 0x00000008
};

void Log(loglevel,const char *text, ...);
void LogInfo(const char *text, ...);

//finish this..
loglevel LogLevel();
void	SetLogLevel(loglevel);

class Autologger {

public:
		Autologger(loglevel,const char *name) {
			fName=name;
			BString start(fName);
			start << " called";
			Log(LOG_INFO,start.String());
		}
		
		~Autologger(){
			
			BString start(fName);
			start << " ended";
			Log(LOG_INFO,start.String());
		
		};
	
	private:
		BString fName;
		
};

#define AUTOLOG	Autologger the_log(LOG_INFO,__PRETTY_FUNCTION__);
#define	LOG(A) the_log.(LOG_INFO,"%s:\n %s\n",__PRETTY_FUNCTION__,A);

#endif

