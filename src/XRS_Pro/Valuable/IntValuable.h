#ifndef _IntValuable_H_
#define _IntValuable_H_

#include "Valuable.h"

template<class BASETYPE, int CAPACITY = 1>
class GenericValuable : public Valuable {
	public:
					GenericValuable() : Valuable(CAPACITY) {
						for (int i=0;i<CAPACITY;i++)
							SetFactor(i, 1.0f); 
					}
					
		float		GetFactorizedValue(int id) 	 		 { return (float)fValue[id]; 	};
		void		SetFactorizedValue(int id,float val) { fValue[id] = (BASETYPE)val;  };	
		
	private:
		BASETYPE	fValue[CAPACITY];
};

typedef GenericValuable<int32, 1> ValuableInt32;
typedef GenericValuable<int16, 1> ValuableInt16;

#endif

//---
