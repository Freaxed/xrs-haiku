#ifndef _XRS_MIDIOUT_LOCALE_
#define _XRS_MIDIOUT_LOCALE_

#ifdef ZETA
#include <locale/Locale.h>
#else
#define _T(A) A 
#endif

#define T_MIDIOUT_NAME			_T("MidiOut")
#define T_MIDIOUT_OUTPUT		_T("Output Channel")
#define T_MIDIOUT_CONTROLLERS	_T("Controllers:")
#define T_MIDIOUT_SEND_CC       _T("Send CC and Bank on PLAY")
#endif