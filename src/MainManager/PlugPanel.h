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
						PlugPanel():BBox(""),curTrack(NULL){};				
		virtual void 	ResetToTrack(Track* tr){curTrack=tr;};
	
	protected:
			void	SetTitle(const char* txt){
			
				if(Window() && txt!=NULL ){
					BString	text(T_TRACKINFO_TITLE);
					text << " - " << txt;
					Window()->SetTitle(text.String());
				
				}
			
			}
	virtual	void	Hide(){
			ResetToTrack(NULL);
			BBox::Hide();
	}
	
	private:
			Track*	curTrack;		
			
	
};
#endif
