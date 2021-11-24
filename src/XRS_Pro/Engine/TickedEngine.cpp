#include "TickedEngine.h"
#include "Logger.h"
#include "LoopClock.h"

LoopClock mainclock;

size_t min(size_t a, size_t b) { return a<b ? a:b; }


TickedEngine::TickedEngine(const char* name):Engine(name) { 
	
	fClock = &mainclock;
	fClock->AddTickable(this);
	SetBPM(120);
	fBeatpos = 0;
}
void	TickedEngine::SetBPM(int bpm){
					fSamplesPerTick=(size_t)2646000/(size_t)bpm;
					fSamplesPerTick /= fClock->Resolution();
					fSamplesPerTick /= 4 ;
					while(fSamplesPerTick % 4 !=0) fSamplesPerTick++;
				}


void
TickedEngine::SecureProcessBuffer(void * buffer, size_t size){ 

	
	const size_t fTot = size/FRAMESIZE;
		
	fLen = fTot;
	
	while(fLen) {
	
		//if(fBeatpos != 0)
		// debugger("trouble");
	
		fBuflen=min(fLen, (int)ceil(fSamplesPerTick-fBeatpos));
		
		//Log(LOG_INFO,"fBuflen: %ld",fBuflen);
		
		
		if(fBuflen >0 && fBuflen<= fTot) {
			FlushEvents((float*)buffer,fBuflen,fTot-fLen); // number of frames : fBuflen.
		}
		
		fBeatpos +=fBuflen; 
		
		if(fBeatpos >= fSamplesPerTick) {			
			fBeatpos -= fSamplesPerTick; //should be always 0!
			if(fBeatpos !=0) debugger("unexpected error! fBeatpos !=0 ");
			if(IsPlaying()) {
				fClock->Tick();
			}
		}
		
		
		fLen-=fBuflen;
		
	}
	FlushAll((float*)buffer,fTot);
	
}

void	
TickedEngine::TickedHigh(uint64 time){
	Log(LOG_INFO,"TickedHigh: global_tick: %lld",time);
	//start new events here!
}

void	
TickedEngine::TickedLow(uint64 time){
	Log(LOG_INFO,"TickedLow: global_tick: %lld",time);
	//stop new events here!
}

void	
TickedEngine::FlushEvents(float* buffer,size_t frames,size_t spiaz){
	Log(LOG_INFO,"FlushEvents: system_time: %lld",system_time());
}	
void	
TickedEngine::FlushAll(float* buffer,size_t frames){
	Log(LOG_INFO,"FlushAll: system_time: %lld",system_time());
	//met=my_mixer->MixDown((float*)buffer,size/FRAMESIZE);
}
