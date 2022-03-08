#ifndef _SFSTrackBoost_H_
#define _SFSTrackBoost_H_

#include	"TrackBoost.h"
#include	"XFSynth.h"
#include	<ObjectList.h>


class SFWindow;	
class BMenu;
class SFSTrack;


class SFSTrackBoost : public TrackBoost
{
	public:
			SFSTrackBoost();
			virtual		~SFSTrackBoost();
	 	Track*		getTrack();
	 	PlugPanel*	getPanel();
	
		bool			Load(Track* cur,int t,BFile* file,int32 va,int32 rt) { return false;};
		void			SaveBoosterSettings(BMessage* data);
		void			LoadBoosterSettings(BMessage* data);
		void			ResetToSong();				//each time a new song is ready
	
	BMenu*	getBankMenu(){ return menu;};
	
	void				SetLine(int line,Track*);
	status_t			LoadSF(const char* filename);
	void				RemoveMe(Track*);
	

	
	private:
		
		BObjectList<SFSTrack>	tracks;
		BMenu*			menu;
		void			_emptyMenu();
		void			ChooseProcessor();

};
#endif

