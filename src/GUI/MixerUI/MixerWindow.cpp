#include "MixerWindow.h"

#include <Application.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>
#include "MixerLine.h"
#include "WindowManager.h"
#include "CommonValuableID.h"
#include "Log.h"
#include "PMixer.h"

MixerWindow*	
MixerWindow::Get()
{
 	static	MixerWindow*	instance = NULL;
	if (instance == NULL) 
			instance = new MixerWindow();
	return instance;
}

MixerWindow::MixerWindow(void)
	: XrsWindow(BRect(150,130,280,60),"", B_FLOATING_WINDOW,B_ASYNCHRONOUS_CONTROLS|B_NOT_ZOOMABLE|B_AVOID_FOCUS|B_NOT_RESIZABLE)
{
	SetName("mixer_");
	SetTitle("Mixer");
	
	BGroupLayout* group = BLayoutBuilder::Group<>(B_HORIZONTAL);
	SetLayout(group);
	group->SetSpacing(5.0f);
	
	for (uint8 i=0; i<PMixer::Get()->CountBusses(); i++) {
		PBus* bus = PMixer::Get()->BusAt(i);
		group->AddView(new MixerLine(bus, VID_MIXER_LIN_VOL(i), VID_MIXER_LIN_PAN(i), VID_MIXER_LIN_MET(i)));
	}
	BSize size(group->BasePreferredSize());
	ResizeTo(size.Width(), size.Height());
	
	WindowManager::Get()->RegisterMe(this,"Mixer");
}

MixerWindow::~MixerWindow()
{
	WindowManager::Get()->UnregisterMe(this);
}

bool
MixerWindow::QuitRequested()
{
	WindowManager::Get()->Switch(this);
	return false;
}
