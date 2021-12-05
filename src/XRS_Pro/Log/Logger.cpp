#include "Logger.h"

#include <stdio.h>

void printLevel(loglevel level){

	switch(level){
		case LOG_INFO:
			printf("INFO: ");   
		break;
		case LOG_WARN:
			printf("WARN: ");
		break;
		case LOG_FATAL:
			printf("FATAL: ");
		break;
		default:
		break;
	};

}

void Log(loglevel level,const char *text, ...){ 
			char buffer[1024]; 
			va_list args; 
   
			va_start(args, text); 
			vsprintf(buffer, text, args); 
			va_end(args); 
			//sender.SendFormat(buffer);
			printLevel(level);
			printf(buffer);
			printf("\n");
			      
}

void LogInfo(const char *text, ...){
			char buffer[1024]; 
			va_list args; 
   
			va_start(args, text); 
			vsprintf(buffer, text, args); 
			va_end(args); 
			
			printLevel(LOG_INFO);
			printf(buffer);
			printf("\n");  
	
}

//--end--
