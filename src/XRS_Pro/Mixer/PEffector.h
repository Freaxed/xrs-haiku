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
#include "Vector.h"

#define	MAX_EFFECT	8

class VSTItem;

class PEffector: public PNode {

	public:

					 PEffector();
			virtual ~PEffector();
			
			int		PMessage(PNode::pnode_message,int);
			size_t	Process(float** data,size_t frames);
			
			void		AddVST(VSTItem *item){ fVstList.Add(item); }
			VSTItem*	VSTAt(int i){ return fVstList[i]; }											 
			uint32	CountEffects(){ return fVstList.Count(); }
	
	private:
			Vector<VSTItem*>	fVstList;
};
#endif
