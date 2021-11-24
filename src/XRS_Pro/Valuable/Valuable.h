#ifndef _Valuable_h_
#define _Valuable_h_

// -------------------------------------------------------------
// is just a super classfor who get/set value
//
// is more:
// is the <model> on the ModelViewController Design Pattern
//
// implemented as a particular tipo of Observerved.
// It notifies (IF registed) it's value to the ValueableManager
// (witch notifies to alla the ValueViews)
// Only a ValueableController can modifies a Valueable
// _____________________________________________________________


#include 	<SupportDefs.h>
#include 	<String.h>


#define	ValuableID		BString
#define VALUABLE_MSG	'valm'

//#include "ValuableManager.h"

#define NullValuable NULL

class Valuable
{
	protected:
				Valuable(int capacity){ if(capacity<=0) capacity=1; factor=new float[capacity]; fCapacity = capacity; };
				
		virtual	float		GetFactorizedValue(int id	   )=0;
		virtual	void		SetFactorizedValue(int id,float)=0;		
		
				float		GetFactor(int id		){ return factor[id];   }
				void		SetFactor(int id,float f){ factor[id]=f; 		}
	public:	
			int32		GetValue(int id);
			void		SetValue(int id,int32 val);
			void		SetValue(int32 val); //same value to all
			
	//protected:
	//friend ValuableManager;
				void	LowSetValue(int id,int32 val); //
				float	LowGetValue(int id);
	protected:
			float*	factor;
			int		fCapacity;			
};

#endif

//-.
