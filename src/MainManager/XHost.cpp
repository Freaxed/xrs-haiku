/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include  "XHost.h"
#include  "XrsMidiOut.h"
#include  "Track.h"
#include  "JuiceEngine.h"
#include  "WindowManager.h"
#include  "MainWindow.h"
#include "Log.h"

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
//	lock = true;
}

void		
XHost::LockEngine(const char* who) 
{
//	if(!lock) 
//		return; //fixme!
	JuiceEngine::Get()->LockEngine(who);
}

void		
XHost::UnlockEngine(const char* who)
{
//	if(!lock) 
//		return; //fixme!
	JuiceEngine::Get()->UnlockEngine(who);
}
		
void*
XHost::SendMessage(int msg,void* param, void* extra)
{
	switch(msg){
	
	case X_MidiProducer:
		return	(BMidiLocalProducer*)XrsMidiOut::Get();
		break;
	
	case X_KillVoice:
		((Track*)extra)->killVoice(param);
		JuiceEngine::Get()->rem_list.AddItem(param);	
		break;
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
	case X_BPM:
		{
		 //int tempo=JuiceEngine::Get()->GetBPM();
		 //memcpy(param,(void*)&tempo,sizeof(int));
		}
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
