#ifndef _MixerEngine_h_
#define _MixerEngine_h_

#include 	"TickedEngine.h"

#include	"Event.h"
#include	"EventList.h"
#include	"Observable.h"


class MediaPart;

class MixerEngine : public TickedEngine, public CObservable {

	public:
	
			enum Operation {
				PLAYING_PART,
				STOPPED_PART
			};
			
						MixerEngine(const char* name);
						~MixerEngine();
			
			void		SetMediaPart(MediaPart*);
			MediaPart*	CurrentPart(){ return fCurrentPart; }
			
			void		StopCurrentPart();
			
		
	protected:
	
			void	Starting();
			void	Stopping();
		
		 	void	TickedHigh(uint64 time);
			void	TickedLow(uint64 time);
		 	
		 	void	FlushEvents(float* buffer,size_t frames,size_t spiaz);	
			void	FlushAll(float* buffer,size_t frames);	

	private:
			
			void			_setUsedPart(uint16);
			MediaPart*		fCurrentPart;
			
			float			*service[2];

};

#endif
