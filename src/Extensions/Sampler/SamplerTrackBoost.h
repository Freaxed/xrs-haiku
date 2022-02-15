/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

/* il loadSample potrebb essere spostato.. no? */

#ifndef _SAMPLER_TRACK_BOOST_H_
#define _SAMPLER_TRACK_BOOST_H_

#include <Menu.h>

#include "TrackBoost.h"

class Sample;
class ExternalManager;
class SamplerTrack;
class SamplerPanel;
class BFilePanel;

class SamplerTrackBoost : public TrackBoost
{
	public:
						SamplerTrackBoost();
						~SamplerTrackBoost();
	 	Track*			getTrack();
		PlugPanel*		getPanel(){return (PlugPanel*)panel;}
		
		void			SaveBoosterSettings(BMessage* data);
		void			LoadBoosterSettings(BMessage* data);
		
	
		status_t	RefReceived(entry_ref,Track*,BMessage *m=NULL);
	

		void			ResetToSong();	//on a new song!
		void			Restart();
		
		void			RemoveAll();
			
		void			ConsiderToRemove(SamplerTrack* t);
	
	ExternalManager*	getEXTM();

	BMenu*				getMenu(){ return menu;};
	void				MakeMenu();
	
	//Samples
	void				RemoveSample(Sample*);
	void				ChangeSample(int id);
	void				LoadSample();
	void				RefreshSelected();
	void				RemoveMe(SamplerTrack*);
	

	protected:
	friend SamplerTrack;

	void	_secureSetSample(SamplerTrack* tr,Sample*);
	
	private:
			
			BList			tracks;
			
			status_t		_checkPath(const char *p);
			status_t		_loadSampler(const char*);
			
			BMenu*		menu;
			ExternalManager*	extm;
			
			BFilePanel*		openpanel;
			entry_ref		sample_ref;
				
			Sample*			FindSample(const char*);

			SamplerPanel	*panel;
			
			
			
};
#endif
