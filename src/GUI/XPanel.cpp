/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
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
#include "VUView.h"

#include <Application.h>

#include <MenuItem.h>
#include <TranslationUtils.h>
#include <InterfaceKit.h>
#include <stdio.h>

BMenuItem* new_bpm_menu(const char* label, int32 bpm_value) {
	BMessage *msg = ValuableTools::CreateMessageForBController(VID_TEMPO_BPM);
	msg->AddInt32("be:value", bpm_value);
	BMenuItem* item =new BMenuItem(label, msg);
	item->SetTarget(ValuableManager::Get());
	return item;
}



XPanel::XPanel(BRect rect): BView(rect, "XPanel", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_NAVIGABLE)
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
XPanel::PlayButtonOn(bool state){
	play_bt->SetValue(state);
}

bool
XPanel::isAllPat(){
	return all_bt->Value();
}


/*Pattern*
XPanel::currentPattern(){
	return curSong->getTrackAt(0)->getPatternAt(curPattern);
}*/

void
XPanel::ResetMeasureCount(){
	curpat->SetMax(curSong->getNumberMeasure());
}

void
XPanel::Reset(Song* s,TracksPanel* f)
{
	curSong = s;
	tp = f;
	
	ValuableManager::Get()->UpdateValue(VID_TEMPO_BPM, curSong->getTempo()); //To be removed!
	
	curpat->UpdateValue(1, true);
	all_bt->SetValue(MeasureManager::Get()->GetPatternMode());
	ResetMeasureCount();
}


void
XPanel::MessageReceived(BMessage* message)
{
	
	if(message->what>1000 && message->what<2000) //?
	{
		
		if(message->what>1900) Window()->PostMessage(message);
		else Window()->PostMessage(new BMessage(GENERIC));
		
	}
		
	switch(message->what)
	{
	
	case CURPAT_MSG:
		message->what=SETPAT;
		message->ReplaceInt32("be:value",message->FindInt32("be:value")-1);
		be_app->PostMessage(message,be_app);
	break;
	
	case SETPAT:
		curpat->UpdateValue(MeasureManager::Get()->GetCurrentPattern()+1, false); 
	break;
	
	case PLAY_START:
		play_bt->SetValue(!play_bt->Value());
	break;
	
	/*case TEMPO_FINE:
		info = new BMessage(TEMPO_MOV);
		info->AddInt32("be:value",bpm->GetValue());
		Window()->PostMessage(info);
	break;*/
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
		BView::MessageReceived(message);
	break;
	}
} 
void
XPanel::AttachedToWindow()
{
	//new stile
	//BBitmap	*tile=XUtils::GetBitmap(6);
	//if(tile!=NULL)  SetViewBitmap(tile);
	
	SetViewColor(panelColor);
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
	AddChild(Two = new ValuablePeakView(VID_MIXER_MAIN_VOL, "PeakView"));
	Two->MoveTo(r.left, r.top);
	Two->ResizeTo(r.Width(), r.Height());
	
	// vol
	r.OffsetBy(37,0);
	r.bottom=r.top+22;
	r.right=r.left+22;
	
	AddChild(master=new XPot(r, "master","xrs.mixer.main.volume", 0, 100, XUtils::GetBitmap(24),NULL));
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
	curpat=new XDigit(r,"curpat", "current_pattern", new BMessage(CURPAT_MSG),1,1);
	AddChild(curpat);
	curpat->SetTarget(this);
	
	//window list	
	r.OffsetBy(55,0);
	r.right=r.left+24;
	AddChild( tool[10] = XUtils::ToolBarButton( r, 5, "tool", new BMessage('winl'), B_ONE_STATE_BUTTON, this));
	tool[10]->SetTarget(this);
		
		
	MakeFocus(false);
	
	MeasureManager::Get()->RegisterMe(this);
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
			
			Window()->PostMessage(new BMessage(PLAY_START));
			play_bt->SetValue(!play_bt->Value());
		
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
