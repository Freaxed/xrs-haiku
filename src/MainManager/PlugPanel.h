#ifndef	_PLUGPANEL_H_
#define	_PLUGPANEL_H_

#include 	<Box.h>
#include	<Window.h>
#include	<String.h>
#include	"locale.h"
#include	"Log.h"

class	Track;

class PlugPanel : public BBox
{
	public:
						PlugPanel(BRect r=BRect(0,0,180,230)):BBox(r,""),curTrack(NULL){};				
		virtual void 	ResetToTrack(Track* tr){curTrack=tr;};
	
	protected:
				void	SetTitle(const char* txt) {} //deprecated
		
		virtual	void	Hide() {
			ResetToTrack(NULL);
			BBox::Hide();
		}
	private:
			Track*	curTrack;		
};
#endif
