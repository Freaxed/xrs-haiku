#include "JMDrumTrackBoost.h"	// *change*
#include "XHost.h"



#define main main_plugin
extern "C" __declspec(dllexport) TrackBoost *main_plugin ();


TrackBoost *main (XHost* x=NULL)
{
	return (TrackBoost*)(new JMDrumTrackBoost);
}
