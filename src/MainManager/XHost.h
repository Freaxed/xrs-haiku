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

// RAII wrapper for XHost engine locking
// Guarantees unlock on all exit paths (exceptions, early returns, normal flow)
class XHostLock {
public:
	explicit XHostLock(const char* who = "XHostLock") 
		: fWho(who)
	{
		XHost::Get()->LockEngine(fWho);
	}
	
	~XHostLock()
	{
		XHost::Get()->UnlockEngine(fWho);
	}
	
	// Prevent copying to avoid double-unlock
	XHostLock(const XHostLock&) = delete;
	XHostLock& operator=(const XHostLock&) = delete;
	
private:
	const char* fWho;
};

