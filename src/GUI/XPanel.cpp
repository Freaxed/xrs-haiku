/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XPanel.h"
#include "GraphicsDef.h"
#include "Xed_Utils.h"
#include "GfxMsg.h"
#include "Track.h"
#include "Song.h"
#include "TracksPanel.h"
#include "ColorsDef.h"
#include "XDigit.h"
#include "XPot.h"
#include "MeasureManager.h"
#include "WindowManager.h"
#include "CommonValuableID.h"
#include "ValuablePeakView.h"

#include <Application.h>

#include <MenuItem.h>
#include <TranslationUtils.h>
#include <InterfaceKit.h>
#include <stdio.h>

BMenuItem* new_bpm_menu(const char* label, int32 bpm_value) 
{
	BMessage *msg = ValuableTools::CreateMessageForBController(VID_TEMPO_BPM);
	msg->AddInt32("be:value", bpm_value);
	BMenuItem* item =new BMenuItem(label, msg);
	item->SetTarget(ValuableManager::Get());
	return item;
}



XPanel::XPanel(BRect rect): BBox(rect, "XPanel", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_NAVIGABLE, B_NO_BORDER)
{
	bpm_menu = new BPopUpMenu("popup",false);
	bpm_menu->AddItem(new_bpm_menu(" 60 bpm",  60));
	bpm_menu->AddItem(new_bpm_menu(" 80 bpm",  80));
	bpm_menu->AddItem(new_bpm_menu("100 bpm", 100));
	bpm_menu->AddItem(new_bpm_menu("120 bpm", 120));
	bpm_menu->AddItem(new_bpm_menu("140 bpm", 140));
	bpm_menu->AddItem(new_bpm_menu("160 bpm", 160));
	bpm_menu->AddItem(new_bpm_menu("180 bpm", 180));
	bpm_menu->AddItem(new_bpm_menu("200 bpm", 200));
	bpm_menu->AddItem(new_bpm_menu("220 bpm", 220));
	bpm_menu->SetLabelFromMarked(false);
	bpm_menu->SetRadioMode(false);

	key_rel=true;
}

void
XPanel::OnPlay(bool isPlaying){
	play_bt->SetValue(isPlaying);
	for (int i=0;i<5;i++)
		tool[i]->SetEnabled(!isPlaying);
}

bool
XPanel::isAllPat(){
	return all_bt->Value();
}

void
XPanel::ResetMeasureCount(){
	LogTrace("XPanel::ResetMeasureCount");
	curpat->SetMax(curSong->getNumberMeasure() - 1);
}

void
XPanel::ResetToSong(Song* s, TracksPanel* f)
{
	curSong = s;
	tp = f;	

	curpat->UpdateValue(0, true);
	all_bt->SetValue(MeasureManager::Get()->GetPatternMode());
	ResetMeasureCount();
}


void
XPanel::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
	case 'bpmp': //show the BPM menu
		{
			BPoint point;
			point.x=tool[9]->Frame().left+2;
			point.y=tool[9]->Frame().bottom+2;
			point=ConvertToScreen(point);
			bpm_menu->Go(point, true, false, true);
			bpm_menu->MakeFocus();
		}
	break;
	case 'winl':
		{
			BPoint point;
			point.x=tool[10]->Frame().left+2;
			point.y=tool[10]->Frame().bottom+2;
			point=ConvertToScreen(point);
			WindowManager::Get()->getMenu()->SetTargetForItems(be_app);
			WindowManager::Get()->getMenu()->Go(point,true,true,true);
		}
	break;
	default:
		BBox::MessageReceived(message);
	break;
	}
} 
void
XPanel::AttachedToWindow()
{
	BRect r(0,0,24,22);
	
	r.OffsetTo(8,8);
	AddChild( tool[0] = XUtils::ToolBarButton( r, 0, "TIP_NEW", new BMessage(MENU_NEW_EMPTY), B_ONE_STATE_BUTTON, this));
	tool[0]->SetTarget(be_app);
	r.OffsetBy(25,0);
	AddChild( tool[1] = XUtils::ToolBarButton( r, 1,"TIP_OPEN", new BMessage(MENU_OPEN), B_ONE_STATE_BUTTON, this));
	tool[1]->SetTarget(be_app);
	r.OffsetBy(25,0);
	AddChild( tool[2] = XUtils::ToolBarButton( r, 2, "TIP_INSERT", new BMessage(MENU_SAVE), B_ONE_STATE_BUTTON, this));
	tool[2]->SetTarget(be_app);
	r.OffsetBy(25,0);
	AddChild( tool[3] = XUtils::ToolBarButton( r, 3, "TIP_SAVE", new BMessage(MENU_SAVEAS), B_ONE_STATE_BUTTON, this));
	tool[3]->SetTarget(be_app);
	r.OffsetBy(25,0);
	AddChild( tool[4] = XUtils::ToolBarButton( r, 4, "TIP_SAVE_SELECTION", new BMessage(MENU_EXPORT), B_ONE_STATE_BUTTON, this));
	tool[4]->SetTarget(be_app);
	
	r.OffsetBy(55,0);
	play_bt=new BPictureButton(r,"play",XUtils::GetPicture(2),XUtils::GetPicture(3),new BMessage(PLAY_START),B_TWO_STATE_BUTTON);
	AddChild(play_bt);
	play_bt->SetTarget(Window());
	
	r.OffsetBy(25,0);
	all_bt=new BPictureButton(r,"all",XUtils::GetPicture(4),XUtils::GetPicture(5),new BMessage(PLAY_ALL_SET),B_TWO_STATE_BUTTON);
	AddChild(all_bt);
	all_bt->SetValue(false);
	all_bt->SetTarget(Window());
	
	
	// mixer
	r.OffsetBy(55,0);
	AddChild( tool[8] = XUtils::ToolBarButton( r, 8, "mixer", new BMessage('VST'), B_ONE_STATE_BUTTON, this));
	tool[8]->SetTarget(Window());
	//meter
	r.OffsetBy(25,0);
	r.right=r.left+36;
	AddChild(Two = new ValuableHPeakView(VID_MIXER_MAIN_MET, "PeakView"));
	Two->MoveTo(r.left, r.top);
	Two->ResizeTo(r.Width(), r.Height());
	
	// vol
	r.OffsetBy(37,0);
	r.bottom=r.top+22;
	r.right=r.left+22;
	
	AddChild(master=new XPot(r, "master", VID_MIXER_MAIN_VOL, 0, 100, XUtils::GetBitmap(24),NULL));
	//r.OffsetBy(0,1);
	//r.bottom=r.top+22;
	
	//bpm
	r.OffsetBy(55,0);
	r.right=r.left+24;
	AddChild( tool[9] = XUtils::ToolBarButton( r, 9, "bpm_menu", new BMessage('bpmp'), B_ONE_STATE_BUTTON, this));
	tool[9]->SetTarget(this);
	//tempo
	r.OffsetBy(25,0);
	r.right=r.left+36;
	AddChild(new XDigit(r, VID_TEMPO_BPM, "bpm-xdigit", NULL, 20, 300));
	
	//playlist
	r.OffsetBy(55,0);
	r.right=r.left+24;
	AddChild( edit = XUtils::ToolBarButton( r, 10, "playlist", new BMessage(EDIT_MSG), B_ONE_STATE_BUTTON, this));
	edit->SetTarget(Window());
	
	r.OffsetBy(25,0);
	r.right=r.left+36;
	curpat = new XDigit(r, VID_PATTERN_CURRENT, "curpattern", NULL, 0, 0);
	curpat->SetDisplayDelta(1);
	AddChild(curpat);
	
	//window list	
	r.OffsetBy(55,0);
	r.right=r.left+24;
	AddChild( tool[10] = XUtils::ToolBarButton( r, 5, "tool", new BMessage('winl'), B_ONE_STATE_BUTTON, this));
	tool[10]->SetTarget(this);
	MakeFocus(false);
	BBox::AttachedToWindow();
}

void
XPanel::KeyUp(const char *key,int32){
	key_rel=true;
}

void
XPanel::KeyDown(const char *key,int32 z)
{	

	if(!key_rel) return;
	key_rel=false;
	BMessage *msg ;
	

	
	switch(key[0])
	{
		case B_ESCAPE:
		Window()->Activate(true);
		break;
		case B_SPACE:
			play_bt->SetValue(!play_bt->Value()); //does this invokes?
			play_bt->Invoke();
		break;	
		
		case B_FUNCTION_KEY:
		
		msg= Window()->CurrentMessage();
		
		if(msg){
			
			int key2=msg->FindInt32("key"); //,&key2);
			if(key2>=2 && key2<=9){				
				key2--;
				curpat->UpdateValue(key2, true);
			}	
		}
		break;	
		
		case 'A':
		case 'a':
			Window()->PostMessage(new BMessage(PLAY_ALL_SET));
			all_bt->SetValue(!all_bt->Value());
		break;
		case 'M':
		case 'm' :
			tp->muteSelectedTrack();
		break;
		
		case 'S':
		case 's' :
			tp->soloSelectedTrack();
		break;
		case B_PAGE_UP:
			curpat->UpdateValue(curpat->GetValue()-1, true);
		break;
		case B_PAGE_DOWN:
			curpat->UpdateValue(curpat->GetValue()+1, true);
		break;
		default:break;
		
	}
	
	if(key[0]>'0' && key[0]<='9'){
	
		int id=key[0]-'1';
		if(id<curSong->getNumberTrack())
		tp->SelectTrack(id);
	
	}
	
		
}
