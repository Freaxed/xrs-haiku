/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <MediaFile.h>
#include <MediaTrack.h>
#include <Entry.h>

#include "VectorNew.h"
#include "GlobalDef.h"

class Sample;
class BFile;

typedef VectorNew<Sample*>	SampleList;


class ExternalManager 
{
	public:
					ExternalManager();
					~ExternalManager();
			
			Sample*			getSampleAt(int);	
			
			void		DeleteSampleAt(int);
			void		DeleteSample(Sample*);


			int32		CountItems();
			status_t	InitCheck();
			void		MakeEmpty();
			status_t	AddSample(entry_ref ref, int *foundPos);
			
			
	private:
		//locked functions
		status_t		LoadFile(entry_ref*, Sample* sample);
	
		SampleList		samples_list;
		status_t		lastStatus;
};
//.
