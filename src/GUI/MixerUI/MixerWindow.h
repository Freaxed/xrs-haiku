#ifndef MIXERWINDOW_H
#define MIXERWINDOW_H

#include "XrsWindow.h"
#include <List.h>

class MixerWindow : public XrsWindow
{
public:
			static	MixerWindow*	Get();
				
				   ~MixerWindow();			
			bool	QuitRequested(void);

			void	ResetUI();

			
private:
					MixerWindow(void);
			BList	mMixerLines;
};

#endif //MIXERWINDOW_H
