#include "InterfaceKit.h"
#include "Entry.h"
// SourceItem

class SourceItem : public BStringItem
{
	public:
		SourceItem(const char* name,entry_ref ref);
		entry_ref	ref;		
		
};
