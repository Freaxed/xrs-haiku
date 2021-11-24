#ifndef _IntValuable_H_
#define _IntValuable_H_

#include "Valuable.h"

class IntValuable : public Valuable 
{
	public:
					IntValuable(int capacity = 1):Valuable(capacity)
					{
						// note, this is quite an hack
						// we use the factor variable to just store and retrive
						// the factorized value. can be usefull? or just messes all up!
					};
					
		float		GetFactorizedValue(int id) 	 { return factor[id]; };
		void		SetFactorizedValue(int id,float val) { factor[id] = val; };	
		
};

#endif

//---
