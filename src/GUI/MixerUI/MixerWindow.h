#ifndef MIXERWINDOW_H
#define MIXERWINDOW_H

#include  "XrsWindow.h"

class MixerWindow : public XrsWindow
{
public:
			static	MixerWindow*	Get();
				
							~MixerWindow();
			
					bool	QuitRequested(void);
			
private:
							MixerWindow(void);
};

#endif //MIXERWINDOW_H
