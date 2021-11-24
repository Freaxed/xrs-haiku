// Message type definitions
#ifndef SYNTHMESSAGES_H_
#define SYNTHMESSAGES_H_

enum SynthMessage {

	TempoChange,	 // samples per sixteenth note
	NoteChange,  	 // change pitch of next note (float freq)
	NoteOn,		 	 // trigger note (float velocity)
	NoteOff,	 	 // release note (float aftertouch?)
	NoteSlide,	 	 // slide to next note (none)
	SystemReset,	 // Make a Reset!
	SystemStop,		 // Stop song
	SystemStart		 // Start song
};

#endif
