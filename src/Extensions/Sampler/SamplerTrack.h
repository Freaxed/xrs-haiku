/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SAMPLERTRACK_H_
#define _SAMPLERTRACK_H_

#include "Track.h"
#include "TrackSampleRateBuffers.h"

class Sample;

#define	 FRAMES_NUM	1024



class SamplerTrack: public Track //, public Refiller
{
	public:
	
						SamplerTrack();
		virtual			~SamplerTrack();
			
				bool		isReversed();
				void		setReversed(bool);
	
			
		//virtual	void		Reset();
		virtual 	int		getModel();	
		//virtual	void		goOn();
		
				 Sample*	getSample();
				void		setSample(Sample*);
				

		//protected??
		
		void		setResample(int);
		int		getResample();
		void		setResampleEnable(bool);
		bool		isResampleEnable();
		void		setBoostEnable(bool b){ boost_enable=b;};
		bool		isBoostEnable(){ return boost_enable;}
		
		virtual TRACK_GEN_TYPE	getProcessorType() { return TT_VOICE_PROCESS; }
		virtual	XRSVoice		newVoice(Note* n,int VoiceTag);
		virtual uint32			ProcessVoice(XRSVoice,float ** buffer ,uint32 sample_num); //,int spiaz);
		virtual	void			killVoice(XRSVoice);
		virtual	void			Message(SynthMessage msg, float data);
		virtual	bool			SupportPanNote(){ return true; }; //tmp
		virtual 	rgb_color	GetPreferredPadColor(){ rgb_color a={49,163,220,255}; return a;}
		//		unsigned long 	read(unsigned char *buffer, unsigned long len,const void* cookie);
		/*RAW*/
		float	amp;
		
	protected:
		virtual	const char*	getPreferredName();
private:

				
		static 		int			min(int a, int b) { return a<b ? a:b; }
					//void			getSampleData(void*,size_t,size_t);
					//void			getSampleData(void*,void*,size_t,size_t);
				
		
		Sample*			curSample;
		bool				reversed;
		
		float				samplesperbeat;
		float				numNotes;
		bool				res_enable;
		bool				boost_enable;
		TrackSampleRateBuffers	mBuffers;
};

#endif
