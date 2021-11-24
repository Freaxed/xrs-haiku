#ifndef _XRS_VIW_LOCALE_
#define _XRS_VIW_LOCALE_

#ifdef ZETA
#include <locale/Locale.h>
#else
#define _T(A) A 
#endif

#define T_VIW_NAME			_T("VSTi Wrap")
#define T_VIW_CANT_FIND		_T("Can't find VSTi :")
#define T_VIW_NOVST			_T("<none>")

#endif
