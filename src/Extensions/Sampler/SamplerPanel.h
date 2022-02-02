/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_SAMPLERPANEL_H_
#define	_SAMPLERPANEL_H_

#include "PlugPanel.h"
#include "SampleView.h"

#include <CheckBox.h>
#include <StringView.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Box.h>
#include <Slider.h>
#include <TabView.h>


class SamplerTrack;	
class SamplerTrackBoost;
class	 Sample;
class XDigit;

class SamplerPanel: public PlugPanel{

	public:
		SamplerPanel(SamplerTrackBoost*);

	 void Reset(Track*);
	 void MessageReceived(BMessage* message);
	 void AttachedToWindow();
		
	private:
		void	SetTrack(SamplerTrack *tr);
		SamplerTrack*	sampTrack;
		
		Sample					*my_sample;
		char txt[30];
		//BStringView 			*text_info;
		SamplerTrackBoost		*booster;
		
		//GUI
		XDigit* 	depth;
		XDigit*  	shift;	
		SampleView*	sw;
		BCheckBox*	rev;
		BCheckBox*	pit_ck;
		BCheckBox*	boost_ck;
		BBox*		rev_box;
		BBox*		pit_box;
		BMenu		*menu;
		BMenuField	*field;
};

#endif
