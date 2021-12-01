#ifndef	_Pannable_H_
#define	_Pannable_H_

#include "Valuable.h"
#include <math.h>

#define	CHANNELS	2

class	Pannable : public Valuable
{
	public:
		Pannable():Valuable(2){ SetFactor(0,127); SetFactor(1,64); pan=0.0f; vol=0.8f; calc_vols();}
		
				float	getVolume(){ return vol; };
		virtual	void	setVolume(float val){ vol=val; calc_vols();};
				float	getPan(){ return pan; };
		virtual	void	setPan(float val){ pan=val; calc_vols();};
	
		void	setLeftRight(float l,float r){
					pan=r-l;
					vol=fabs(pan);
					vols[0]=l;
					vols[1]=r;
				};
		
		float		GetLeft(){ return vols[0]; };
		float		GetRight(){ return vols[1]; };
		
	protected:
	virtual	float		GetFactorizedValue(int id){  if(id==0) return getVolume(); else return getPan(); };
	virtual	void		SetFactorizedValue(int id,float v){ if(id==0) setVolume(v); else setPan(v);};
		
			float vols[CHANNELS];

	private:
		 float	pan;
		 float	vol;
		 
		 //from vol to left/right vol
		 inline	void 	calc_vols(){
		 
		 			vols[0] = vol;
					vols[1] = vol;
		
					if(pan>0) 
						vols[0]*=(1.0f - pan);
					else
					if(pan<0) 
						vols[1]*=(1.0f + pan);
		 
		 };
 
};
#endif
