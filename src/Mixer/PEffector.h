/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */
/*
	PEffector : A child from PEffector and Pannable.
*/
#ifndef _PEffector_H_
#define _PEffector_H_

#include "PNode.h"
#include "assert.h"

#define	MAX_EFFECT	5

class VSTItem;

class PEffector: public PNode {

	public:

					 PEffector();
			virtual ~PEffector();
			
			int		PMessage(PNode::pnode_message,int);

			size_t	Process(float** data,size_t frames);

			VSTItem*	VSTAt(uint8 i){ assert(i<MAX_EFFECT); return fVstStack[i]; }											 
	
	private:
			VSTItem*				fVstStack[MAX_EFFECT];
};
#endif
