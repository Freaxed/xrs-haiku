/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

/*
	PBus : A PMixable with effector.
*/
#ifndef _PBus_H_
#define _PBus_H_

#include "PMixable.h"
#include "PEffector.h"

#include "VectorNew.h"


class PBus: public PMixable  {

	public:

					 
					 					
			virtual	int		PMessage(PNode::pnode_message,int);
			virtual	size_t	Process(float** data,size_t frames);


			virtual	void	Process(PBus* caller,size_t frames);	//FIX change name!
					

					
					void	AddRouted(PBus* routed){ fRouted.Add(routed); }
					
					PEffector*	Effector(){ return &fEffector; }
					
					void	    SetName(const char* name) { fName = name; }
					const char*	Name() { return fName.String(); }
				
	private:
			
			PEffector 			fEffector;
			VectorNew<PBus*>	fRouted;
			BString				fName;
};
#endif
