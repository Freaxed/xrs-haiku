#ifndef _XRS_SAMPLER_LOCALE_
#define _XRS_SAMPLER_LOCALE_

#ifdef ZETA
#include <locale/Locale.h>
#else
#define _T(A) A 
#endif

#define T_SAMPLER_NAME						_T("Sampler")
#define T_SAMPLER_NAME_MENU					_T("Samples..")
#define	T_SAMPLER_REMOVE_ALL_SAMPLES		_T("Remove All the samples?")
#define	T_SAMPLER_REMOVE_SAMPLE_1			_T("Remove Sample")
#define	T_SAMPLER_REMOVE_SAMPLE_2			_T("from ALL tracks using it?")
#define T_SAMPLER_CANT_FIND					_T("Could not find Sample:")
#define T_SAMPLER_CANT_BANK					_T("Could not find Bank: ")
#define T_SAMPLER_UNSET						_T("<un-set>")

//sampler panel
#define T_SAMPLER_LOAD						_T("Load a Sample..")
#define T_SAMPLER_REMOVE					_T("Remove THIS Sample..")
#define T_SAMPLER_REMOVE_ALL				_T("Remove ALL Samples..")

#define T_SAMPLER_STRECH					_T("Strech")
#define T_SAMPLER_BOOST						_T("Boost")
#define T_SAMPLER_REVERSE					_T("Reverse")
#define T_SAMPLER_NOSELECTED				_T("No sample selected")

#endif
