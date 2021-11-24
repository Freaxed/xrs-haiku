#include <View.h>
//#include <Synth.h>

class KickView : public BView
{
	public:
		
		KickView(BRect);
	
	virtual void Draw(BRect);
		  void Init(void *data,int32,bool);
	virtual void AttachedToWindow();
		  void SetReversed(bool v){rev=v;Invalidate();}
		  void SetAmp(float v){amp=v;Invalidate();}
			
	private:
		int16	left;
		int16	right;
		float		left_p,right_p;
		int32	sampleCount;
		void		*data;
		bool 		rev;
		float 	zero;
		float 	incy;
		int 		incx;
		float		amp;
		
};

