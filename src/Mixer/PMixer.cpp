/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PMixer.h"
#include "ValuableManager.h"
#include "VstManager.h"

PMixer* 
PMixer::Get()
{
	static	PMixer*	instance = NULL;
	if (instance == NULL) 
			instance = new PMixer();
	return instance;
}


PMixer::PMixer(){ 
	
	BusAt(0)->SetName("Line 0 (Master)");
	
	for(int i=1; i<MIXERLINES_COUNT; i++){
		BString name("Line ");
		name << i;
		busses[i].SetName(name.String());
		BusAt(0)->AddRouted(&busses[i]);	
	}	
	BusAt(0)->ClearBuffer();
	
}

PMixer::~PMixer()
{

}


void	
PMixer::ResetBuffers()
{
	for(int i=0;i<MIXERLINES_COUNT;i++) {
		if(busses[i].Used())
		{
			busses[i].ClearBuffer();
			busses[i].SetUsed(false);
		}
	}
}
PBus*	
PMixer::GetMain() {
	return BusAt(0);
}
				
PBus*	
PMixer::BusAt(uint8 pos){
	if (pos >= MIXERLINES_COUNT) 
		return NULL;
		
	return &busses[pos];
}

 //Mixer related, triggered by JuiceEngine!
void	
PMixer::ValuableChanged(BMessage* msg)
{
	ValuableID vID;
	int32 value;
	if (msg->FindInt32(VAL_DATA_KEY, 0, &value) != B_OK) {
		return;
	}	
	uint8 busKey = vID.ByteAt(BASE_VID_MIXER_LINE.Length()) - '0';

	if (busKey >= MIXERLINES_COUNT)
		return;

	if(vID.FindFirst(BASE_VID_VOLUME, BASE_VID_MIXER_LINE.Length() + 1) != B_ERROR) {
		BusAt(busKey)->SetGain((float)value/100.0f);
	} else if(vID.FindFirst(BASE_VID_PAN, BASE_VID_MIXER_LINE.Length() + 1) != B_ERROR) {
		BusAt(busKey)->SetPan((float)value/100.0f);
	}	
}

void
PMixer::SaveSettings(BMessage& mixer)
{
	for(int i=0;i<MIXERLINES_COUNT;i++) {
		BMessage mixerLine;

		BMessage gainValue;
		ValuableManager::Get()->GetInternalLastMessage(VID_MIXER_LIN_VOL(i), gainValue);
		mixerLine.AddMessage( "Gain", &gainValue);

		BMessage panValue;
		ValuableManager::Get()->GetInternalLastMessage(VID_MIXER_LIN_VOL(i), panValue);
		mixerLine.AddMessage( "Pan",  &panValue);
		
		BMessage fxs;
		BusAt(i)->Effector()->SaveSettings(fxs);
		mixerLine.AddMessage("Effects", &fxs);

		mixer.AddMessage("Line", &mixerLine);
	}
}

void
PMixer::LoadSettings(BMessage& mixer)
{
	int i=0;
	BMessage mixerLine;
	while (mixer.FindMessage("Line", i, &mixerLine) == B_OK)
	{
		if (i >= MIXERLINES_COUNT)
			break;

		BMessage gainValue;
		if (mixerLine.FindMessage("Gain", &gainValue) == B_OK)
			ValuableManager::Get()->UpdateValue(gainValue);

		BMessage panValue;
		if (mixerLine.FindMessage("Pan", &panValue) == B_OK)
			ValuableManager::Get()->UpdateValue(panValue);
			
		BMessage fxs;
		mixerLine.FindMessage("Effects", &fxs);
		BusAt(i)->Effector()->LoadSettings(fxs);

		i++;
	}
}

//---
