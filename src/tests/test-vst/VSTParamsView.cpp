#include "VSTParamsView.h"
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>
#include "VSTKnob.h"
#include <StringView.h>

#define MAX_PER_ROW 10

VSTParamsView::VSTParamsView(VSTPlugin* plugin)
              : BView("VSTParamsView", B_WILL_DRAW|B_FRAME_EVENTS, NULL)
{
	BGroupLayout* group = BLayoutBuilder::Group<>(B_VERTICAL);
	SetLayout(group);
	BGridLayout* grid = BLayoutBuilder::Grid<>();
	//SetLayout(grid);
	group->AddView(new BStringView("_fx_name_", plugin->EffectName()))
		 ->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_VERTICAL_CENTER));
	group->AddItem(grid);
	grid->SetSpacing(1, 1);
	
	int count = plugin->ParametersCount();
	for (int p = 0; p < count; p++) {
		VSTKnob* knob = new VSTKnob(plugin->Parameter(p));
		grid->AddView(knob, p % MAX_PER_ROW, p / MAX_PER_ROW);
	}

	ResizeTo(group->BasePreferredSize());
}

void VSTParamsView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
}
