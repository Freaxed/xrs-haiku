#ifndef _SEV_H_
#define _SEV_H_

#include "Note.h"
//#include "Sample.h"

class Sample;

struct ev {
		Note*		n;
		size_t		ap;
		Sample	*	sp;
	};		
	
#endif

//-
