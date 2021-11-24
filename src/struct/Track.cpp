#include "Track.h"
#include "XHost.h"
#include "Pattern.h"
#include "Note.h"

extern XHost*	xhost;
   
Track::Track ():Pannable()
{
	on=true;
	route_line=0;
	curNote=NULL;
	name_set=false;
	cit=true;		
	midiin_channel=0;
	midiin_enable=false;
	ResetName();
}

void
Track::setNumberNotes(int n)
{
	for(int h=0 ;  h<patternList.CountItems(); h++)  
	
		((Pattern*)patternList.ItemAt(h))->setNumberNotes(n);
}


void
Track::setName(const char* t)
{
	if(!name_set)	
		name.SetTo(t);
	/*else
		printf("__ setName, but name locked!\n");*/
}

const char*
Track::getPreferredName()
{
	return "unset_";
}
void
Track::ResetName()
{
	name.SetTo(getPreferredName());
	LockName(false);
}
void
Track::LockName(bool b)
{
	name_set=b;
}
const char*
Track::getName()
{
	return name.String();
}
bool
Track::isNameLocked()
{
	return name_set;
}
/*void 
Track::Message(SynthMessage msg, float data)
{
	switch(msg) {
		
		case SystemReset:
			Reset();
			return;
		break;
		default: break; // gcc is dumn
	}
}*/

void
Track::AddMeasure(int val,int note)
{
	for(int h=0 ;  h<val; h++)  
	
		 patternList.AddItem(new Pattern(note));
		 
	currentPattern=(Pattern*)patternList.ItemAt(0);	 
}
void
Track::RemoveMeasure(int num)
{
	 patternList.RemoveItem(num);
		 
	currentPattern=(Pattern*)patternList.ItemAt(0);	 
}

Track::~Track(){}
int	Track::getModel(){	return 0; }
bool 
Track::isOn()
{
	return on;
}
void
Track::setOn(bool val)
{
	on=val;
	if(val) goOn(); else goOff();
}

void
Track::moveToPattern(int val)
{
	currentPattern=(Pattern*)patternList.ItemAt(val);	
}

Pattern*
Track::getPatternAt(int val)
{
	return (Pattern*)patternList.ItemAt(val);		
}

Pattern*
Track::getCurrentPattern()
{
	return currentPattern;
}

void
Track::Reset()
{
	curNote=NULL;
	
}
void 	Track::Message(SynthMessage msg, float data){};
void Track::Process(float ** buffer ,int32 sample_num,int spiaz,float factor){}
