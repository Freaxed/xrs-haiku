/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include <stdio.h>

enum 
{
		X_MidiProducer,
		X_BPM,
		X_KillVoice,
		X_LockSem,
		X_UnLockSem,
		X_RegWindow,
		X_UnregWindow,
		X_Switch,
		X_MainWindowDeactivate,
		X_MainWindowActivate
};

class XHost{

	public:
		static	XHost*	Get();
		
		void*		SendMessage(int msg, void* param, void* extra=NULL);
		void		AllowLock(bool b){ lock=b; }
		
	private:
				XHost();
		bool	lock;		 	
};

//extern	XHost*				xhost;
