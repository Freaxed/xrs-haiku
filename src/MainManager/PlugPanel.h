#ifndef	_PLUGPANEL_H_
#define	_PLUGPANEL_H_

#include 	<Box.h>
#include	<Window.h>
#include	<String.h>
#include	"locale.h"

class	Track;

class PlugPanel : public BBox
{
	public:
				PlugPanel(BRect r=BRect(0,0,180,230)):BBox(r,""),curTrack(NULL){};
	virtual void 	Reset(Track* tr){curTrack=tr;};
	
	protected:
			void	SetTitle(const char* txt){
			
				if(Window() && txt!=NULL ){
					BString	text(T_TRACKINFO_TITLE);
					text << " - " << txt;
					Window()->SetTitle(text.String());
				
				}
			
			}
	virtual	void	Hide(){
			Reset(NULL);
			BBox::Hide();
	}
		private:
			Track*	curTrack;		
	
};
#endif
