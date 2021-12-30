#include "VSTParamsView.h"
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include "VSTKnob.h"

VSTParamsView::VSTParamsView(VSTPlugin* plugin)
              :BBox (B_EMPTY_STRING, 0)
{
	BGridLayout* grid = BLayoutBuilder::Grid<>();
	int count = plugin->ParametersCount();

	for (int p = 0; p < count; p++) {
		grid->AddView(new VSTKnob(plugin->Parameter(p)), p%5, p/5);		
	}
	
	AddChild(grid->View());
}
