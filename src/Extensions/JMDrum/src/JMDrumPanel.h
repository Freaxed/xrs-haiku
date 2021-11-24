#include 	"PlugPanel.h"
#include	<Menu.h>
#include	<MenuItem.h>
#include	<MenuField.h>
#include	<Box.h>
#include	<Message.h>
#include	<List.h>
#include	<StringView.h>


class	XDigit;
class	JMDrumTrack;
class	BSlider;



class JMDrumPanel: public PlugPanel
{
	public:
				JMDrumPanel();
	virtual void 	Reset(Track* tr);
	virtual void	AttachedToWindow();
	virtual void	MessageReceived(BMessage *msg);
	
	private:
		
		JMDrumTrack*	myTrack;
		BSlider*		sl[16];
		BStringView*	txt[16];
		BString	display;
		
};
//--
