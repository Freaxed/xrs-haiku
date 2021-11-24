/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <MediaFile.h>
#include <MediaTrack.h>
#include <Entry.h>

#include "Vector.h"
#include "GlobalDef.h"

class Sample;
class BFile;

typedef Vector<Sample*>	SampleList;
typedef Vector<Sample*>::Iterator	SampleListIterator;	

class ExternalManager 
{
	public:
					ExternalManager();
					~ExternalManager();
			
			Sample*			getSampleAt(int);	
			SampleList*		getSampleList();
			
			status_t		AddSampleList(BList*);

			int32		CountItems();
			status_t	InitCheck();
			void		Empty();
			status_t	AddSample(entry_ref ref,int *pos=NULL);
			status_t	AddBankSample(BMessage*);
			
			entry_ref	the_ref; //ugly, fix!
			
			status_t	ExtractSample(entry_ref,int32);
	private:
	
		//locked functions
		status_t		LoadFile(entry_ref*, int *pos = NULL);
		void*			MemoryResampler(void* data,uint32 frames ,int ch,uint32 freq);
		
		status_t		_extractSample(entry_ref,int32);
		
		SampleList				samples_list;
		status_t				lastStatus;
};
//.
