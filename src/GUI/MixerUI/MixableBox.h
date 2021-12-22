/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef MixableBox_H_
#define MixableBox_H_

#include <Box.h>

#include "XChannelSlider.h"
#include "XPot.h"
#include "GfxMeter.h"
#include "FixedText.h"
#include "ValuableManager.h"

class PMixable;

class MixableBox : public BBox {
	
	public:
			MixableBox(BPoint position, BString name, ValuableID volume, ValuableID pan, ValuableID meter);
			
	private:

			FixedText*	fName;
			XChannelSlider*	fSlider;
			XPot*			fPot;
			GfxMeter*		fMeter;
};

#endif

//--
