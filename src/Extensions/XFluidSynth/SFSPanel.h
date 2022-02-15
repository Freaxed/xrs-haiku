#include 	"PlugPanel.h"
#include	<Menu.h>
#include	<MenuItem.h>
#include	<MenuField.h>
#include	<Box.h>
#include	<Message.h>
#include	<List.h>
#include	<StringView.h>


//#include 	"fluidsynth.h"

class		ADigit;
class		APot;
class		SFSTrack;
class		BPictureButton;

class SFSPanel: public PlugPanel
{
	public:
				SFSPanel();
	virtual void 	ResetToTrack(Track* tr);
	virtual void	AttachedToWindow();
	virtual void	MessageReceived(BMessage *msg);
		 	void	Refresh();
		  
	private:
		ADigit*			ch;
		APot*			reverb;
		APot*			chorus;
		SFSTrack*	myTrack;
		BMenu*			menu;
		BPictureButton*	brez;
		BRect	but;
		BBox 	*sampler_t;
		BMenuField *field;
};
