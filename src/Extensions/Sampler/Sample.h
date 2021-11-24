/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "GlobalDef.h"
#include <SupportKit.h>

enum
{
	UNDEFINED=0,
	MONO = 1,
	STEREO = 2
};

class Sample 
{
	public:
			Sample();
			~Sample();
		
		void		PrintToStream();
		short*		wave_data;
		
		char   		name[SAMPLE_NAME_L];
			
		uint32		_totalbytes;
		uint32		frames;
		uint32		offset;		
		int			modifier;
		float 		freq_divisor;
		int			type;	// 0 is BANK Samples
							// 1 is External Sample //
		char*		path_name;
		// Annotazioni:
		
		/*
		
		Sarebbe bello avere un Sample
		fatto di float e con un numero di channels
		variabili.
		Per fare questo però è necessario avere prima un
		convertitore di frequenza che lavora sui 
		float e non sui short.
		
		Per ora è bene tenersi l'ottimo
		Resempler che c'e'...
		poi vedremo di sistemarlo..
		
		*/
		
		//float*		dataF;
		int			channels;

};

#endif
