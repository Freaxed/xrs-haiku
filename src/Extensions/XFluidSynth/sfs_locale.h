#ifndef _XRS_SAMPLER_LOCALE_
#define _XRS_SAMPLER_LOCALE_

#ifdef ZETA
#include <locale/Locale.h>
#else
#define _T(A) A 
#endif

#define T_SFS_ERROR_LOADING		_T("Error loading SoundFont")
#define T_SFS_LOADING			_T("Loading soundfont..")

#define T_SFS_NO_SF2			_T("no sf2 loaded!")
#define T_SFS_REVERB			_T("Reverb")
#define	T_SFS_LEVEL				_T("Level")
#define	T_SFS_WIDTH				_T("Width")
#define T_SFS_DAMPING			_T("Damping")
#define T_SFS_ROOMSIZE			_T("Roomsize")
#define T_SFS_GAIN				_T("Gain")
#define T_SFS_CHORUS			_T("Chorus")
#define T_SFS_NR				_T("Nr")
#define T_SFS_DEPTH				_T("Depth")
#define T_SFS_SPEED				_T("Speed")
#define T_SFS_WAVE_UNSET		_T("<unset>")
#define T_SFS_WAVE_SINE			_T("sinewave")
#define T_SFS_WAVE_TRIANGLE		_T("triangle")
#define T_SFS_WAVE_TYPE			_T("Type")
#define T_SFS_CHANNEl			_T("Channel")


#endif
