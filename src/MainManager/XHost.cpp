/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */
#include  "XHost.h"
#ifdef XRS_MIDI
	#include  "XrsMidiOut.h"
#endif
#include  "Track.h"
#include  "JuiceEngine.h"
#include  "WindowManager.h"
#include  "MainWindow.h"
#include  "Log.h"

XHost*
XHost::Get()
{
	static	XHost*	instance = NULL;
	if (instance == NULL) 
			instance = new XHost();
	return instance;
}

XHost::XHost()
{

}

void		
XHost::LockEngine(const char* who) 
{
	JuiceEngine::Get()->LockEngine(who);
}

void		
XHost::UnlockEngine(const char* who)
{
	JuiceEngine::Get()->UnlockEngine(who);
}
		
void*
XHost::SendMessage(int msg,void* param, void* extra)
{
	switch(msg){
#ifdef XRS_MIDI
	case X_MidiProducer:
		return	(BMidiLocalProducer*)XrsMidiOut::Get();
		break;
#endif
	case X_RegWindow:
		WindowManager::Get()->RegisterMe((BWindow*)param,(const char*)extra);	
		break;
	case X_UnregWindow:
		WindowManager::Get()->UnregisterMe((BWindow*)param);	
		break;
	case X_Switch:
		WindowManager::Get()->Switch((BWindow*)param);	
		break;
	case X_LockSem:
		LockEngine();
		break;
	case X_UnLockSem:
		UnlockEngine();
		break;			
	case X_MainWindowDeactivate:
		if(MainWindow::Get()->Lock())
		{
			MainWindow::Get()->WindowActivated(false);
			MainWindow::Get()->Unlock();
		} 
		break;		
	case X_MainWindowActivate:	
		if(MainWindow::Get()->Lock())
		{
			MainWindow::Get()->WindowActivated(true);
			MainWindow::Get()->Unlock();
		} 
		break;		
	default:
		return NULL;
		break;
	}
	
	return NULL;
	
}
