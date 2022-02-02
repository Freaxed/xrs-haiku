/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

/*
	PNode: An abstract ProcessingNode.
*/
#ifndef _PNode_H_
#define _PNode_H_

#include <SupportDefs.h>

#define MAX_FRAMES		1024
#define	NUM_CHANNELS	2
#define FRAMESIZE		8

class PNode {

	public:

		enum pnode_message {
			
			P_INIT,
			P_RESET,
			P_START,
			P_STOP,
			P_PANIC,
			P_TEMPO
		
		};


					 PNode(){ next=NULL; SetUsed(false); };
			
			virtual	int		PMessage(PNode::pnode_message msg,int val) {
							if(next) 
								return next->PMessage(msg,val);
							else 
								return 0;		
			}
			
			virtual	size_t	Process(float** data,size_t frames) {
			
							if(next) 
								 return next->Process(data,frames);
							else
								return 0;
			}
			
			void	SetUsed(bool u){ m_used = u; }
			bool	Used(){ return m_used; }
					
			
			PNode*	next;
	
	protected:
		bool	  m_used;
			
		
};
#endif

