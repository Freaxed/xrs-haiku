#include "ValuableRecorder.h"
#include "Application.h"
#include "Bitmap.h"
#include "Logger.h"
#include "TranslationUtils.h"

#include "stdio.h"
#include "ValuableManager.h"




#define		MAXR	200
float		recorded[MAXR];
int 	fPos;
bool	done=false;



ValuableRecorder::ValuableRecorder(BRect r):BStringView(r,"name","label") {
	fPos=0;
}


void	
ValuableRecorder::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case MSG_VALUABLE_CHANGED:
		{
			if(fPos>=MAXR || done) return;
			
			float value;
			//int id;
			
			msg->FindFloat("valuable:value",&value);
			
			
			//id=msg->FindInt16("valuable:value:id");
			if(fPos>0 && recorded[fPos-1] == value) return;
			
			recorded[fPos]=value;
			{	
				BString val;
				val << fPos;
				SetText(val.String());
			}
			
			fPos++;
		}
		break;
		
		
		
		default:
		BStringView::MessageReceived(msg);	
		break;
	}
}

void	
ValuableRecorder::AttachedToWindow(){
	BStringView::AttachedToWindow();		
}
