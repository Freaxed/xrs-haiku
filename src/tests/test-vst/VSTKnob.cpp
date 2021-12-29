
#include "VSTKnob.h"
#include "VSTHost.h"

VSTKnob::VSTKnob(VSTParameter* parameter):fParameter(parameter) {
	SetParameterString(fParameter->Name());
	SetValue(fParameter->Value());
};

void
VSTKnob::SetValue(float newValue)
{
	fParameter->SetValue (newValue);
	ParameterValueKnob::SetValue(newValue);
	char	complete[128];
	sprintf (complete, "%s %s", fParameter->CurrentValue(), fParameter->Unit());
	SetValueString (complete);
}
