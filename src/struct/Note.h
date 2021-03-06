/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#ifndef _NOTE_H_
#define _NOTE_H_

#include	"StereoVolumes.h"

class Note: public StereoVolumes
{
	public:
	
				Note();
		bool 	getValue();
		void	setValue(bool);
		void	Swap();
		void	setOct(int);
		int		getOct();
		long	getNote(){return note;};
		void	setNote(long v){note=v;};

		void 	LoadSettings(BMessage& msg);
		void 	SaveSettings(BMessage& msg);

	private:

		bool	value;
		int		oct;  // maschera di bit che decide : ottava,slide,accento
		long	note;
};

#endif
