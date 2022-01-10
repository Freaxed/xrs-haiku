
#include "VSTKnob.h"
#include "VSTHost.h"

VSTKnob::VSTKnob(VSTParameter* parameter): ParameterValueKnob(parameter->Name()) , fParameter(parameter) {
	SetLabelText(fParameter->Name());
	SetValue(fParameter->Value());
};

void
VSTKnob::SetValue(float newValue)
{
	fParameter->SetValue (newValue);
	ParameterValueKnob::SetValue(newValue);
}

void
VSTKnob::ValueDisplay(float value, BString& display)
{
	display << fParameter->CurrentValue() << " " << fParameter->Unit();
}

void		
VSTKnob::LoadParameter()
{
	ParameterValueKnob::SetValue(fParameter->Value());
}
