#ifndef _JTrack_H
#define _JTrack_H

#include "TrackBlock.h"
#include <GraphicsDefs.h>
//

class		Pattern;
class		XNotesView;
class 		XTrack;
class		Track;
class		BPopUpMenu;
class		APot;
class		XDigit;
class		BPictureButton;

#define	XRS_SIMPLE_DATA	'xrsd'
#define	FORCE_ROUTELINE	'FFRL'


class JTrack : public TrackBlock
{
	public:
				JTrack(BRect, int16);
		
		virtual void	AttachedToWindow();
			  	void	SetID(int16 id);

			  	void	ResetToTrack(Pattern*, Track*, int16 beatDivisio);
			  	void	Refresh(Pattern*, int16 beatDivision);
			    void	SetSelected(bool selected);
			  	int		getModel();
			  	void	Mute(bool,bool);
				void  	MouseDown(BPoint where);
		
		
			  void	SetName(const char*);
			  
			  void	InvokeRename(); //simulate ctrl+click on xtr!
			 
			  void	MessageReceived(BMessage*);
			 
			
			 Track*	getTrack() {return myTrack;};
	protected:

			Pattern*		myPat;
			Track*			myTrack;
			APot*			volpot;
			APot*			panpot;
			XNotesView*		vnc;
			XTrack*			xtr;
			int16			id;
			int				model;
			

	private:
	
			void				RControl();
			int					_img;	
			BPictureButton*		brez;
			XDigit*				line_pos;
};

#endif
