#ifndef _TickedEngine_h_
#define _TickedEngine_h_

#include "Engine.h"
#include "Tickable.h"
#include "Clock.h"

class TickedEngine : public Engine, public Tickable {

	public:
	
						TickedEngine(const char* name);
			
	protected: //?
	
		virtual void	SecureProcessBuffer(void * buffer, size_t size);
		
		virtual	void	TickedHigh(uint64 time);
		virtual	void	TickedLow(uint64 time);
		
		virtual	void	FlushEvents(float* buffer,size_t frames,size_t spiaz);
		virtual	void	FlushAll(float* buffer,size_t frames);	
		
				
		virtual void	SetBPM(int bpm);
		
		
	protected:
		Clock*			fClock;
		size_t			fBeatpos;
		//tick lenght:
		size_t	fSamplesPerTick;
	
	private:

			
			//processing variables:
			size_t	 		fLen;
			//size_t			fTot;
			size_t  		fBuflen;
			
		
			
};

#endif
