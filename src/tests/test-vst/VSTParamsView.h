#ifndef VSTPARAMSVIEW_H
#define VSTPARAMSVIEW_H

#include <Box.h>
#include "VSTHost.h"

class VSTParamsView : public BBox {
public:
		VSTParamsView(VSTPlugin* plugin);
};

#endif
