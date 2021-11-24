#include "Valuable.h"
//#include "ValuableManager.h"


int32		
Valuable::GetValue(int id){	return (int32)(GetFactorizedValue(id)*factor[id]);};


void		
Valuable::SetValue(int32 val){ 
	for(int i=0;i<fCapacity;i++)
		SetValue(i,val);
};

void		
Valuable::SetValue(int id,int32 val){ 

	LowSetValue(id,val);
	//fix deve mandare un BMESSAGE
	//fVManager->ValuableChanged("mixer.master.volume");

};


void		
Valuable::LowSetValue(int id,int32 val){ 
	SetFactorizedValue(id,(float)(val/factor[id])); 
};

float	
Valuable::LowGetValue(int id){
	return GetFactorizedValue(id);
};


//--
