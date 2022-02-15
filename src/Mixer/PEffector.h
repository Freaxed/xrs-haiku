/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
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
#include <assert.h>
#include <Message.h>

#define	MAX_EFFECT	5

class VSTItem;
class BList;
class VSTPlugin;

class PEffector: public PNode {

	public:

					 PEffector();
			virtual ~PEffector();
			
			int		PMessage(PNode::pnode_message,int);

			size_t	Process(float** data,size_t frames);

			void	SaveSettings(BMessage& msg);
			void	LoadSettings(BMessage& msg);
			
			VSTItem*		CreateVstAtPosition(VSTPlugin* templ, uint8 pos); 	
			VSTItem*		GetVstAtPosition(uint8 pos);
			//VSTPlugin list: template list!
			const BList*	GetEffectsList() { return fFxList; } 
	
	private:
			VSTPlugin*				FindVST(const char* name);
			VSTItem*				_LockedSwap(VSTItem* newPlugin,  uint8 pos);
			VSTItem*				fVstStack[MAX_EFFECT];
			const BList*			fFxList;
			
};
#endif
