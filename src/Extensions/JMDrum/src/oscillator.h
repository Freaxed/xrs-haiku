// oscillator.h: interface for the Coscillator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OSCILLATOR_H__8DCC6093_4904_4A88_BE80_EED313A8F713__INCLUDED_)
#define AFX_OSCILLATOR_H__8DCC6093_4904_4A88_BE80_EED313A8F713__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TWOPI 6.283185307179586476925286766559

#define MAX_ENVPOS TWOPI
//#define TO2PI TWOPI/MAX_ENVPOS


// Class Oscillator is NOT a real oscillator. It's a container that automatically increments
// its position after a call to GetPos() (and sets it back to 0 if exceeded MAX_ENVPOS);
// Returns values between 0 and MAX_ENVPOS in a linear increment.

class Coscillator  
{
public:
	double GetPos()		// RETURNS VALUES BETWEEN 0 and MAX_ENVPOS
	{
		const double cupos=envpos;
	
		envpos+=incremspeed;
		if (envpos>MAX_ENVPOS) envpos-=MAX_ENVPOS;
		return(cupos);
	}
	void setEnvPos(float epos);		// 0<=epos<=MAX_ENVPOS
	void setHz(float nhz)			// theoretically any frequency but...
	{
		incremspeed=nhz*hztosamples;
	}
	void setSampleRate(int nsamplerate);  // samplerate in Hz NOTE: changing samplerate needs
	int getSampleRate()					// that you recall setHz() or (set)IncSpeed(in)() after.
	{
		return(hz);
	}
	void setIncSpeed(double nincspeed)	// POSITIVE VALUES ALWAYS!
	{
		incremspeed=nincspeed;
	}
	void IncSpeedin(double nspeed)		// If negative value, higher than incremspeed so that
	{									// final value is still positive.
		incremspeed+=nspeed;
	}
	Coscillator();
	virtual ~Coscillator();

	double incremspeed;

protected:
	int hz;					// it's only use is to return the value of int getSamplerate();
	float hztosamples;
	double envpos;

};

#endif // !defined(AFX_OSCILLATOR_H__8DCC6093_4904_4A88_BE80_EED313A8F713__INCLUDED_)
