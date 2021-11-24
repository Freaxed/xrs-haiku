#include "ValuablePlayer.h"
#include "Application.h"
#include "Bitmap.h"
//#include "Bitmaps.h"
#include "TranslationUtils.h"

#include "stdio.h"
#include "ValuableManager.h"

extern float		recorded[200];
extern int fPos;
extern bool done;

ValuablePlayer::ValuablePlayer()
{
	//Run();
	SendSender(NULL);
	
	//SendValue(0,Value());
}


void
ValuablePlayer::play(){

	done = true;
	//SendValue(0,recorded[0]);
	
	for(int i=0;i<fPos;i++)
	{
		SendValue(0,recorded[i],10000*i);
		//snooze(100);
	}
	fPos=0;
}


