
#include "MachineInterface.h"
#include "Drum.h"
#include <String.h>

#include "jmdrum_locale.h"

#define DRUM_VERSION "2.2"
#define MAX_TRACKS 2 //64
#define MAX_SIMUL_TRACKS 2 //16
#define NUMPARAMETERS 16


CMachineParameter const prStartFreq = {
	"Start Freq",
	"Start Frequency (Hz)",
	80,
	600,
	MPF_STATE,
	200
};

CMachineParameter const prEndFreq = {
	"End Freq",
	"End Frequency (Hz)",
	20,
	400,
	MPF_STATE,
	44
};
CMachineParameter const prFreqDecay = {
	"Freq Decay",
	"Frequency Down Speed (in mscs)",
	10,
	300,
	MPF_STATE,
	65
};
CMachineParameter const prStartAmp = {
	"Start Amp (1)",
	"Starting Amplitude",
	0,
	32767,
	MPF_STATE,
	32767
};
CMachineParameter const prEndAmp = {
	"End Amp (1)",
	"Ending Amplitude",
	0,
	32767,
	MPF_STATE,
	21844
};

CMachineParameter const prLength = {
	"Length",
	"Duration of the note (ms)",
	10,
	500,
	MPF_STATE,
	180
};
CMachineParameter const prOutVol = {
	"Volume",
	"Volume (0-32767)",
	0,
	32767,
	MPF_STATE,
	32767
};
CMachineParameter const prDecMode = {
	"Dec Mode",
	"Decrement Mode",
	0,
	3,
	MPF_STATE,
//	2
	0
};
CMachineParameter const prComp = {
	"Note sensitive",
	"NoteOn Sensitive",
	0,
	1,
	MPF_STATE,
//	1
	0
};
CMachineParameter const prNNA = {
	"NNA Command",
	"NNA Command when New Note",
	0,
	1,
	MPF_STATE,
	0
};

CMachineParameter const prAttack = {
	"Attack up to (2)",
	"Attack from 0/100 to x/100",
	0,
	99,
	MPF_STATE,
	2
};
CMachineParameter const prDecay = {
	"Decay up to (2)",
	"Decay from Attack to x/100",
	1,
	100,
	MPF_STATE,
	63
};
CMachineParameter const prSustain = {
	"Sustain Volume (2)",
	"Sustain Volume at Decay-End Pos",
	0,
	100,
	MPF_STATE,
	55
};
CMachineParameter const prMix = {
	"Drum&Thump Mix",
	"Mix of Drum and Thump Signals",
	-100,
	100,
	MPF_STATE,
//	-37
	-100
};
CMachineParameter const prThumpLen = {
	"Thump Length",
	"Thump Length",
	1,
	60,
	MPF_STATE,
	10
};
CMachineParameter const prThumpFreq = {
	"Thump Freq",
	"Thump Frequency",
	220,
	6000,
	MPF_STATE,
	5000
};
CMachineParameter const prNothing = {
	"-",
	"Nothing",
	0,
	1,
	MPF_STATE,
	0
};

CMachineParameter const *pParameters[] = 
{ 
	&prStartFreq,
	&prEndFreq,
	&prFreqDecay,
	&prStartAmp,
	&prEndAmp,
	&prLength,
	&prOutVol,
	&prDecMode,
	&prComp,
	&prNNA,
	&prAttack,
	&prDecay,
	&prSustain,
	&prMix,
	&prThumpLen,
	&prThumpFreq,
};

CMachineInfo const MacInfo =
{
	MI_VERSION,
	GENERATOR,
	NUMPARAMETERS,
	pParameters,
#ifdef _DEBUG
	"Drum Synth v." DRUM_VERSION " (Debug)",
#else
	"Drum Synth v." DRUM_VERSION,
#endif
	"Drum" DRUM_VERSION,
	"[JAZ] on " __DATE__,
	"Command Help",
	4
};


BString DescribeValue(int const param, int const value);
bool DescribeValue(char* txt,int const param, int const value);
void ParameterTweak(int par, int val,int* Vals,DrumPars *globalpar);

//Drum only;


void ParameterTweak(int par, int val,int* Vals,DrumPars *globalpar)
{
	// Called when a parameter is changed by the host app / user gui
	Vals[par]=val;
	switch (par) {
		case 0: 
			globalpar->StartSpeed=val * (float)(MAX_ENVPOS)/44100.0;
			globalpar->IncSpeed= (Vals[1] - val) * (float)(MAX_ENVPOS)/ (globalpar->DecLength*44100.0);
			break;
		case 1:
			globalpar->IncSpeed= (val - Vals[0]) * (float)(MAX_ENVPOS)/ (globalpar->DecLength*44100.0);
			break;
		case 2:
			globalpar->DecLength=val * 44100.0 / 1000.0;
			globalpar->IncSpeed= (Vals[1] - Vals[0]) * (float)(MAX_ENVPOS)/ (globalpar->DecLength*44100.0);
			break;
		case 3:
			if ( Vals[8] == 0 ) {
				globalpar->SinVol=val*globalpar->sinmix/100.0;
				globalpar->DecayDec=((val-Vals[4])*globalpar->sinmix)/(globalpar->SLength*100.0);
			}
			break;
		case 4:
			if ( Vals[8] == 0 ) {
				globalpar->DecayDec=((Vals[3]-val)*globalpar->sinmix)/(globalpar->SLength*100.0);
				//globalpar->SustainDec; // <- Not Used
			}
			break;
		case 5:
			globalpar->SLength=(int)(val* 44100)/1000;
			if ( Vals[8] == 0 ) {
				//globalpar->AttackPos; globalpar->AttackInc; //<- Not Used
				//globalpar->DecayPos=globalpar->SLength;
				globalpar->DecayDec=((Vals[3]-Vals[4])*globalpar->sinmix)/(globalpar->SLength*100.0);
				//globalpar->SustainDec; // <- Not Used
			}
			else {
				if ( Vals[10] != 0 ) { // If it's 0, they don't change.
					globalpar->AttackPos=globalpar->SLength*Vals[10]/100;
					globalpar->AttackInc=(327.67*(float)globalpar->sinmix)/(float)globalpar->AttackPos;
				}
				if ( Vals[10] > Vals[11] ) globalpar->DecayPos=globalpar->AttackPos+1;
				else globalpar->DecayPos=globalpar->SLength*Vals[11]/100;
				globalpar->DecayDec=((327.67-(3.2767*Vals[12]))*globalpar->sinmix)/(globalpar->DecayPos-globalpar->AttackPos);
				globalpar->SustainDec=(3.2767*Vals[12]*globalpar->sinmix)/(globalpar->SLength-globalpar->DecayPos);
			}
			break;
		case 6:	globalpar->OutVol=val/32767.0;
			break;
		case 7:	globalpar->DecMode=val;
			break;
		case 8: 
			if ( val == 0 ) {
				globalpar->AttackPos=0; // globalpar->AttackInc; //<- Not used
				globalpar->SinVol=Vals[3]*globalpar->sinmix/100.0;
				globalpar->DecayPos=globalpar->SLength;
				globalpar->DecayDec=((Vals[3]-Vals[4])*globalpar->sinmix)/(globalpar->SLength*100.0);
				//globalpar->SustainDec; // <- Not Used.
			}
			else {
				if ( Vals[10] != 0 ) {
					globalpar->AttackPos=(int)(globalpar->SLength*Vals[10]/100.0);
					globalpar->AttackInc=(327.67*(float)globalpar->sinmix)/(float)globalpar->AttackPos;
					globalpar->SinVol=0;
				}
				else {  
					globalpar->AttackPos=0; // globalpar->AttackInc; //<- Not used
					globalpar->SinVol=327.67*globalpar->sinmix;
				}
				if ( Vals[10] > Vals[11] ) globalpar->DecayPos=globalpar->AttackPos+1;
				else globalpar->DecayPos=globalpar->SLength*Vals[11]/100;
				globalpar->DecayDec=((327.67-(3.2767*Vals[12]))*globalpar->sinmix)/(globalpar->DecayPos-globalpar->AttackPos);
				globalpar->SustainDec=(3.2767*Vals[12]*globalpar->sinmix)/(globalpar->SLength-globalpar->DecayPos);
			}
			break;
		case 9: break;
		case 10:
			if ( Vals[8] == 1 ) {
				if ( val != 0 ) { 
					globalpar->AttackPos=globalpar->SLength*val/100;
					globalpar->AttackInc=(327.67*(float)globalpar->sinmix)/(float)globalpar->AttackPos;
					globalpar->SinVol=0;
				}
				else {  
					globalpar->AttackPos=0;
					globalpar->SinVol=327.67*globalpar->sinmix;
				}
			}
			break;
		case 11:
			if ( Vals[8] == 1 ) {
				if ( Vals[10] > val ) globalpar->DecayPos=globalpar->AttackPos+1;
				else globalpar->DecayPos=globalpar->SLength*val/100;
				globalpar->DecayDec=((327.67-(3.2767*Vals[12]))*globalpar->sinmix)/(globalpar->DecayPos-globalpar->AttackPos);
				globalpar->SustainDec=(3.2767*Vals[12]*globalpar->sinmix)/(globalpar->SLength-globalpar->DecayPos);
			}
			break;
		case 12:
			if ( Vals[8] == 1 ) {
				globalpar->DecayDec=((327.67-(3.2767*val))*globalpar->sinmix)/(globalpar->DecayPos-globalpar->AttackPos);
				globalpar->SustainDec=(3.2767*val*globalpar->sinmix)/(globalpar->SLength-globalpar->DecayPos);
			}
			break;
		case 13: 
			if ( val > 0 ) { globalpar->sinmix=(-1)*(val-100); globalpar->ThumpVol=32767; }
			else { globalpar->sinmix=100; globalpar->ThumpVol=327.67*(val+100); }
			if ( globalpar->AttackPos == 0 ) {
				if ( Vals[8] == 0 ) globalpar->SinVol=Vals[3]*globalpar->sinmix/100.0;
				else globalpar->SinVol=327.67*globalpar->sinmix;
			}
			else {
				globalpar->SinVol=0;
				globalpar->AttackInc=(327.67*(float)globalpar->sinmix)/(float)globalpar->AttackPos;
			}
			if ( Vals[8] == 0 ) {
				globalpar->DecayDec=((Vals[3]-Vals[4])*globalpar->sinmix)/(globalpar->SLength*100.0);
			}
			else {
				globalpar->DecayDec=((327.67-(3.2767*Vals[12]))*globalpar->sinmix)/(globalpar->DecayPos-globalpar->AttackPos);
				globalpar->SustainDec=(3.2767*Vals[12]*globalpar->sinmix)/(globalpar->SLength-globalpar->DecayPos);
			}
			globalpar->ThumpDec=globalpar->ThumpVol/globalpar->ThumpLength;
			break;
		case 14: 
			globalpar->ThumpLength=(int)((val* 44100.0)/10000.0);
			globalpar->ThumpDec=globalpar->ThumpVol/globalpar->ThumpLength;
			break;
		case 15:
			globalpar->ThumpFreq=val;
		default:break;
	}
}

BString
DescribeValue(int const param, int const value)
{
	BString out("");
	
	switch(param) {
		case 0:
		case 1:
		case 15:
			out << value << "Hz";
		break;
		case 2:
		case 5:
			out << value << "ms";
		break;
		case 7:
			switch(value) {
				case 0: //sprintf(txt,"Linear");
				out << "Linear";
				break;
				case 1: //sprintf(txt,"Mode 1.1");
				out << "Mode 1.1";
				break;
				case 2: //sprintf(txt,"Mode 2.0");
				out << "Mode 2.0";
				break;
				default: //sprintf(txt,"No Decrease");
				out << "No Decrease";
				break;
			}
		break;
		case 8:
			out << (value==0?"Off":"On");
		break;
		case 9:
			out << (value==0?"NoteOff":"NoteContinue");
		break;
		case 10:
		case 11:
		case 12:
			//sprintf(txt,"%d%%",value);
			out << value << "%";
		break;
		case 13:
			if ( value< 0 ) out << "100%:" << (value+100) << "%"; //sprintf(txt,"100%%:%d%%",(value+100));
			//else sprintf(txt,"%d%%:100%%",(value-100)*(-1));
			else out  << (value-100)*(-1) << "%" << ":100%";
		break;
		case 14:
			//sprintf(txt,"%0.1fms",value/10.0); return true;
			out << (float)(value/10.0) << "ms";
		break;
		default: break;
	}
	
	return out;
}
CMachineInfo const * GetInfo(){ return &MacInfo; }
