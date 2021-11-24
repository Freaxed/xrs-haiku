/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include <MidiConsumer.h>
#include <List.h>

class BControl;

class Track;
class Song;

class XrsMidiIn : public BMidiLocalConsumer
{
	public:
					
	 		void		NoteOn(uchar channel, uchar note, uchar velocity,bigtime_t time);
	 		void		NoteOff(uchar channel, uchar note, uchar velocity,bigtime_t time);
	 		void		ControlChange(uchar channel, uchar controlNumber,uchar controlValue,bigtime_t time);					
			void		RegisterCh(Track* t,int i);
			void		UnregisterCh(Track* t,int i);			
			void		RegisterCC(BControl*,int ch);
			
			void		Reset(Song*);
			
			static	XrsMidiIn*	Get();
	
	private:
					 XrsMidiIn();
			BList	 channels[16];
};
