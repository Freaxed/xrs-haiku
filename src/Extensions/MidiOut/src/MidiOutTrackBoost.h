#ifndef _SAMPLER_TRACK_BOOST_H_
#define _SAMPLER_TRACK_BOOST_H_

#include "TrackBoost.h"

class BMidiLocalProducer;
class	MidiPanel;

class MidiOutTrackBoost : public TrackBoost
{
	public:
			MidiOutTrackBoost();
	virtual	~MidiOutTrackBoost() {};
	
	virtual 	Track*	getTrack();
	virtual	PlugPanel*	getPanel();

	virtual	void			LoadTrackSettings(Track* trk,BMessage* data);
	virtual	void			SaveTrackSettings(Track* trk,BMessage* data);
	virtual 	const char*	getPreferredName() {return "Midi Out";} 

	private:
			
			BMidiLocalProducer*		mider;
			MidiPanel*		mp;
			
};
#endif