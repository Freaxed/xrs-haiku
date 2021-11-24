#include "resample.h"
class Sample;
#include "SamplerVoice.h"


class SampleRefiller: public Refiller {
public:
	SampleRefiller(Sample* s)
	{
	 sample=s;
	 position=0;
	}
	
	
	unsigned long read(unsigned char *buffer, unsigned long len,const void* cookie)
	{
		SamplerVoice*	Voice=(SamplerVoice*)cookie;
		
		if(Voice->refiller!=this) debugger("no way");
		
		if(position+len > sample->_totalbytes)
		{
			len = sample->_totalbytes-position;
			//printf("position %ld total %ld\n",position+len,sample->_totalbytes);
		}
		if(len>0)
		{
			if(Voice->reverse)
			{
				//short* buf=(short*)buffer;
				short val;
				//printf("ch: %d tot byte %ld tot frames %ld -- len %ld, postion %ld spiaz %ld\n",sample->channels,sample->_totalbytes,sample->frames,len,position,sample->frames-((position/2)+0)-1);
				
				for(uint i=0;i < len/2 ;i++)
				{
					val=sample->wave_data[sample->frames-((position/2)+i)-1];
					memcpy(buffer+i*2,(char*)&val,2);
				}
				
			}
			else
			memcpy(buffer,(char*)sample->wave_data+(int)position,len);
			
			
			position +=len;
			return len;
		}
		//printf("Name %s %s",((SamplerVoice*)cookie)->sample->name,sample->name);
		return 0;
		
	};
	
	
	
	private:
	Sample*			sample;
	uint32 			position; // in bytes!!!!!!!!!!
};

//--
