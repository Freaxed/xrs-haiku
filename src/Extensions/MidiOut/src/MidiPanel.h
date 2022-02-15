#include "PlugPanel.h"
#include  <MenuField.h>
#include	<PopUpMenu.h>

class		XDigit;
class		MidiOutTrack;
class 	APot;
class		BCheckBox;
class 	BMenuField;

class MidiPanel: public PlugPanel
{
	public:
				MidiPanel();
	 void 	ResetToTrack(Track* tr);
	 void	AttachedToWindow();
	 void	MessageReceived(BMessage *msg);
	
	private:
		typedef struct controller{
			APot*	value;
			XDigit*	number;
		};
		XDigit*	ch,*ch2;
		MidiOutTrack*	myTrack;
		BMenu *menu;
		controller	my_cont[4];
		BCheckBox*	send_ck;
		BMenuField*	field;
};
