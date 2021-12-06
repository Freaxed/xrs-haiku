/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef ValuableManager_h
#define ValuableManager_h

#include	<Window.h>

#include "Valuable.h"
#include "ValuableView.h"

#include "VectorMap.h"
#include "Vector.h"

class ValuableManager : public BLooper
{

	public:
						
					   ~ValuableManager();
				static  ValuableManager* Get();
				
				bool	RegisterValuable(ValuableID, Valuable*);
				bool	RegisterValuableView(ValuableID, ValuableView*, bool update=true);
				
				void	UnRegisterValuable(ValuableID);
				void	UnRegisterValuableView(ValuableID,ValuableView*);
	
				float	RetriveValue(ValuableID,int channel = 0);
				
				ValuableID	FindValuableID(Valuable*); //slow!
				
				void MessageReceived(BMessage* msg);
				
				void	SpreadValue(ValuableID,int ch,float value);	
				
				// warning this is not thread safe.
				void	DirectSetValuable(ValuableID, int channel, int32 value);
				
				//don't like..				
				BMessenger	*messenger;
				
				void	DumpValues();
				
								
	private:
					ValuableManager();
	
			void	_ValuableChanged(BMessage* msg);
			void	_SendMessage(ValuableView*,BMessage* msg);
			
			typedef	Vector<ValuableView*>	ValList;
				
			struct valuable_list {
					Valuable*	value;
					ValList		list;
			};
			
			VectorMap<ValuableID, valuable_list*>	map;
			typedef VectorMap<ValuableID, valuable_list*>::Iterator	iterator;
};

//extern ValuableManager*		fVManager;


#endif
