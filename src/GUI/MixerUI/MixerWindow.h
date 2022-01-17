#ifndef MIXERWINDOW_H
#define MIXERWINDOW_H

#include  "XrsWindow.h"

class MixerWindow : public XrsWindow
{

public:
			static MixerWindow*	Get();
				
						~MixerWindow();
			
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
			void		SaveSettings(BMessage*	data);
			void		LoadSettings(BMessage*	data);
			
private:
						MixerWindow(void);
};

#endif //MIXERWINDOW_H
