#include "ValuableMonitorWindow.h"

#include <Application.h>
#include <Box.h>
#include <Slider.h>
#include <TextView.h>

#define H_LABEL 30

ValuableMonitorWindow::ValuableMonitorWindow(void)
	:	BWindow(BRect(100,100,500,400),"ValuableMonitorWindow",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	BBox* mainBox = new BBox(BRect(0,0,80,300), "mainBox", B_FOLLOW_LEFT);

	volume = new BSlider(BRect(20,20,60,250), "volume", "volume", NULL , 0, 100);
	volume->SetModificationMessage(ValuableTools::CreateMessageForBController("xrs.mixer.main.volume"));
	volume->SetOrientation(B_VERTICAL);
	volume->SetTarget(ValuableManager::Get());

	mainBox->AddChild(volume);
	AddChild(mainBox);	

}


void 
ValuableMonitorWindow::RegisterValuable(ValuableID vID) {
	if (Lock()){
		if (mValuableLabel.find(vID) == mValuableLabel.end()) {
			int32 count = mValuableLabel.size();
			mValuableLabel[vID] = new BTextView(BRect(81, 0 + (H_LABEL*count), 400 - 81, H_LABEL + (H_LABEL*count)), vID.String(), BRect(0,0,400-81,60), B_FOLLOW_LEFT);
			mValuableLabel[vID]->SetText(vID.String());
			AddChild(mValuableLabel[vID]);
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
			int32 newValue;
			if (ValuableTools::SearchValues("xrs.mixer.main.volume", msg, &newValue))
				volume->SetValue(newValue);
				
			ValuableID vID;
			if (msg->FindString(VAL_ID, &vID) == B_OK) {
				if (mValuableLabel.find(vID) != mValuableLabel.end()) {
					mValuableLabel[vID]->SetText(GetValuableString(vID, msg));
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
