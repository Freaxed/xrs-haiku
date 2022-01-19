#ifndef _STRINGBOX_H
#define _STRINGBOX_H

#include <ControlLook.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <MenuItem.h>

class StringBox : public BMenuField
{
	public:
		StringBox(const char* label, BPopUpMenu* popUp, BMessage* extraInfo) : BMenuField("_fMenu_field_", NULL, new BMenu("")) 
		{
			fPopUp = popUp;
			fExtraInfo = *extraInfo;
		}
		
		void KeyDown(const char* bytes, int32 numBytes) {

			BView::KeyDown(bytes, numBytes);
		}
		
		void MouseDown(BPoint where)
		{
			BView::MouseDown(where);

			BPoint location(Bounds().left, Bounds().bottom);
			ConvertToScreen(&location);
			BMenuItem* item = fPopUp->Go(location);
			if (item)  {
				
				if (item->Message()) {
					BMessage copy(*item->Message());
					copy.AddMessage("extra", &fExtraInfo);
					copy.AddPointer("box", this);
					Looper()->PostMessage(&copy, item->Target());
				}
			}

		}
		
		void	
		UpdateLabel(const char* name)
		{
			Menu()->Superitem()->SetLabel(name);
		}
		
		void
		AllAttached()
		{
			BMenuField::AllAttached();
			SetExplicitSize(BSize(100.0f, Bounds().Height()));
		}
		
		BPopUpMenu*	fPopUp;
		BMessage	fExtraInfo;
		
};
#endif //_STRINGBOX_H
