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

class SamplerTrack: public Track
{
	public:
	
						SamplerTrack();
		virtual			~SamplerTrack();
			
				bool		isReversed();
				void		setReversed(bool);
	

			 	int		getModel();	

		
			Sample*		getSample();
			  void		setSample(Sample*);
				
				
		void				SaveCustomSettings(BMessage& msg);
		void				LoadCustomSettings(BMessage& msg);
				

		//protected??
		
		void		setResample(int);
		int			getResample();
		void		setResampleEnable(bool);
		bool		isResampleEnable();
		void		setBoostEnable(bool b){ boost_enable=b;};
		bool		isBoostEnable(){ return boost_enable;}
		
		bool		IsLoopEnable() {return loop_enable;}
		void		SetLoopEnable(bool b) {loop_enable = b;}
		
		TRACK_GEN_TYPE	getProcessorType() { return TT_VOICE_PROCESS; }
		XRSVoice		newVoice(Note* n,int VoiceTag);
		uint32			ProcessVoice(XRSVoice,float ** buffer ,uint32 sample_num); //,int spiaz);
		void			killVoice(XRSVoice);
		void			Message(SynthMessage msg, float data);
		bool			SupportPanNote(){ return true; }; //tmp
		rgb_color	GetPreferredPadColor(){ rgb_color a={49,163,220,255}; return a;}

		float	amp; //FIXME, encapsulate.
		
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
		bool				loop_enable;
		TrackSampleRateBuffers	mBuffers;
};

#endif
