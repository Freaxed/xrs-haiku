#include "ValuableMonitorWindow.h"

#include <Application.h>
#include <Box.h>
#include <Slider.h>
#include <TextView.h>
#include <ScrollView.h>
#include <ListView.h>

#define H_LABEL 20

ValuableMonitorWindow::ValuableMonitorWindow(void)
	:	BWindow(BRect(100,100,500,400),"ValuableMonitorWindow",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fValuePanel = new BListView(BRect(0, 0, 400 - B_H_SCROLL_BAR_HEIGHT, 300 - B_V_SCROLL_BAR_WIDTH), "fValuePanel", B_SINGLE_SELECTION_LIST);
	
	fScrollView = new BScrollView("scroll", fValuePanel , B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS, true, true, B_FANCY_BORDER);

	AddChild(fScrollView);

}


void 
ValuableMonitorWindow::RegisterValuable(ValuableID vID) {
	if (Lock()){
		if (mValuableLabel.find(vID) == mValuableLabel.end()) {
			mValuableLabel[vID] = new BStringItem(vID);
			fValuePanel->AddItem(mValuableLabel[vID]);
			ValuableManager::Get()->RegisterValuableReceiver(vID, this);
		}
	
		Unlock();
	}
}
void 
ValuableMonitorWindow::UnregisterValuable(ValuableID vID) {
	if (Lock()){
		if (mValuableLabel.find(vID) != mValuableLabel.end()) {
			mValuableLabel[vID]->SetText("<unregistered>");
		}
		Unlock();
	}
}



void
ValuableMonitorWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case MSG_VALUABLE_CHANGED:
		{
			ValuableID vID;
			if (msg->FindString(VAL_ID, &vID) == B_OK) {
				if (mValuableLabel.find(vID) != mValuableLabel.end()) {
					mValuableLabel[vID]->SetText(GetValuableString(vID, msg));
					fValuePanel->InvalidateItem(fValuePanel->IndexOf(mValuableLabel[vID]));
				}
			}
		}
		break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

BString ValuableMonitorWindow::GetValuableString(ValuableID vID, BMessage* input) {

		vID << ":";
		type_code typeFound = -1;
		int32 countFound = 1;
		if(input->GetInfo(VAL_DATA_KEY,&typeFound, &countFound) != B_OK)
				return "error";	
		
		for (int i=0;i<countFound;i++){
			switch(typeFound) {
				case B_FLOAT_TYPE:
				{
					float data;
					if (input->FindFloat(VAL_DATA_KEY, i, &data) == B_OK) {
						vID << " " << data;
					}
				}
				break;
				case B_INT32_TYPE:
				{
					int32 data;
					if (input->FindInt32(VAL_DATA_KEY, i, &data) == B_OK) {
						vID << " " << data;
					}
				}
				break;
			}
		}	
		return vID;
}


bool
ValuableMonitorWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
