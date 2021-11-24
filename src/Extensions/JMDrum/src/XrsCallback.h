#ifndef  XRS_CALLBACK_H_
#define XRS_CALLBACK_H_

#include "MachineInterface.h"
#include "stdio.h"

#define INFO printf("******************* XrsCallback: %s\n",__FUNCTION__);
class XrsCallback : public CFxCallback
{
public:
		XrsCallback(){};
	virtual void MessBox(char* ptxt,char*caption,unsigned int type)
	{
	INFO	
	}
	virtual int CallbackFunc(int cbkID,int par1,int par2,int par3){INFO return 0;}
	virtual float *GetWaveLData(int inst,int wave){INFO return 0;} // USELESS if you cannot get the length!
	virtual float *GetWaveRData(int inst,int wave){INFO return 0;} // USELESS if you cannot get the length!
	virtual int GetTickLength(){INFO return 1024;}
	virtual int GetSamplingRate(){ return 44100;}
	virtual int GetBPM(){INFO return 125;}
	virtual int GetTPB(){INFO return 4;}
	// Don't get fooled by the above return values.
	// You get a pointer to a subclass of this one that returns the correct ones.
};

#endif