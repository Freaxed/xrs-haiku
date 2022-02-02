/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "MidiProducer.h"

class XrsMidiOut : public BMidiLocalProducer
{
	public:
		static XrsMidiOut*	Get();

	private:
			XrsMidiOut();
};

//-
