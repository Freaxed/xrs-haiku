/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include <stdio.h>

enum 
{
#ifdef XRS_MIDI
	X_MidiProducer,
#endif
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
		void		LockEngine  (const char* who = "XHost");
		void		UnlockEngine(const char* who = "XHost");
		
	private:
				XHost();
};

