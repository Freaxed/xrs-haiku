#ifndef _SFSTrackBoost_H_
#define _SFSTrackBoost_H_

#include	"TrackBoost.h"
#include	"XFSynth.h"
#include	"ObjectList.h"


class SFWindow;	
class BMenu;
class SFSTrack;


class SFSTrackBoost : public TrackBoost
{
	public:
			SFSTrackBoost();
			virtual		~SFSTrackBoost();
	virtual 	Track*		getTrack();
	virtual 	PlugPanel*	getPanel();
	
	virtual	bool			Load(Track* cur,int t,BFile* file,int32 va,int32 rt) { return false;};
	virtual	void			SaveTrackSettings(Track* trk,BMessage* data);
	virtual	void			SaveBoosterSettings(BMessage* data);
	virtual	void			LoadBoosterSettings(BMessage* data);
	virtual	void			LoadTrackSettings(Track* trk,BMessage* data);
	virtual	void			Reset();				//each time a new song is ready
	virtual	void			Restart();				//EACH TIME A song is going to be close.
	
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

