#include "SourceItem.h"

SourceItem::SourceItem(const char*name,entry_ref rif):BStringItem(name)
{
	ref=rif;
}