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
#include "XDigit.h"
#include "ColorsDef.h"
#include "XDigit.h"
#include "XPot.h"
#include	"MeasureManager.h"
#include	"WindowManager.h"

#include "VUView.h"

#include <Application.h>

#include <MenuItem.h>
#include <TranslationUtils.h>
#include <InterfaceKit.h>
#include <stdio.h>
	



XPanel::XPanel(BRect rect): BView(rect,"_xthis",B_FOLLOW_ALL_SIDES,B_WILL_DRAW | B_NAVIGABLE)
{

	
	bpm_menu=new BPopUpMenu("popup",false);
	
	BMessage *msg=new BMessage('bpms');
	bpm_menu->AddItem(new BMenuItem(" 60 bpm",msg));
	bpm_menu->AddItem(new BMenuItem(" 80 bpm",msg));
	bpm_menu->AddItem(new BMenuItem("100 bpm",msg));
	bpm_menu->AddItem(new BMenuItem("120 bpm",msg));
	bpm_menu->AddItem(new BMenuItem("140 bpm",msg));
	bpm_menu->AddItem(new BMenuItem("160 bpm",msg));
	bpm_menu->AddItem(new BMenuItem("180 bpm",msg));
	bpm_menu->AddItem(new BMenuItem("200 bpm",msg));
	bpm_menu->AddItem(new BMenuItem("220 bpm",msg));
	
	key_rel=true;
	//SetEventMask(B_KEYBOARD_EVENTS);
	
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
	curSong=s;
	//curPattern=0;
	tp=f;
	bpm->SetValue(curSong->getTempo());
	curpat->SetValue(1);
	all_bt->SetValue(MeasureManager::Get()->GetPatternMode());
	ResetMeasureCount();
}


void
XPanel::MessageReceived(BMessage* message)
{

	BMessage *info;
	
	if(message->what>1000 && message->what<2000)
	{
		
		if(message->what>1900) Window()->PostMessage(message);
		else Window()->PostMessage(new BMessage(GENERIC));
		
	}
		
	switch(message->what)
	{
	
	/*case OBSERVED:
		master->SetValue(message->FindInt32("be:value"));
		//temp->SetValue();
	break; */
	case CURPAT_MSG:
		message->what=SETPAT;
		message->ReplaceInt32("be:value",message->FindInt32("be:value")-1);
		be_app->PostMessage(message,be_app);
	break;
	
	case SETPAT:
		curpat->SetValue(MeasureManager::Get()->GetCurrentPattern()+1,false);
	break;
	
	case PLAY_START:
		play_bt->SetValue(!play_bt->Value());
	break;
	
	case TEMPO_FINE:
		info = new BMessage(TEMPO_MOV);
		info->AddInt32("be:value",bpm->GetValue());
		Window()->PostMessage(info);
	break;
	case 'bpms':
		{
			int32 val=message->FindInt32("index")*20+60;
			bpm_menu->FindMarked()->SetMarked(false);
			info=new BMessage(TEMPO_MOV);
			info->AddInt32("be:value",val);
			bpm->SetValue(val);
			Window()->PostMessage(info);
		}
	break;
	case 'bpmp':
		{
			BPoint point;
			point.x=tool[9]->Frame().left+2;
			point.y=tool[9]->Frame().bottom+2;
			point=ConvertToScreen(point);
			bpm_menu->Go(point,true,true,true);
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
	AddChild(One = new VUView(r,"vumeter")); //"mixer.master.meter"	
	
	
	// vol
	r.OffsetBy(37,0);
	r.bottom=r.top+22;
	r.right=r.left+22;
	
	AddChild(master=new XPot(r, "master","mixer.master",0,NULL,0, 127,XUtils::GetBitmap(24),NULL));
	//r.OffsetBy(0,1);
	//r.bottom=r.top+22;
	
	//bpm
	r.OffsetBy(55,0);
	r.right=r.left+24;
	AddChild( tool[9] = XUtils::ToolBarButton( r, 9, "bpm", new BMessage('bpmp'), B_ONE_STATE_BUTTON, this));
	tool[9]->SetTarget(this);
	//tempo
	r.OffsetBy(25,0);
	r.right=r.left+36;
	AddChild(bpm=new XDigit(r,"bmp",new BMessage(TEMPO_FINE),NULL,20,300));
	
	//playlist
	r.OffsetBy(55,0);
	r.right=r.left+24;
	AddChild( edit = XUtils::ToolBarButton( r, 10, "playlist", new BMessage(EDIT_MSG), B_ONE_STATE_BUTTON, this));
	edit->SetTarget(Window());
	
	r.OffsetBy(25,0);
	r.right=r.left+36;
	curpat=new XDigit(r,"curpat",new BMessage(CURPAT_MSG),NULL,1,1);
	AddChild(curpat);
	curpat->SetTarget(this);
	
	//window list	
	r.OffsetBy(55,0);
	r.right=r.left+24;
	AddChild( tool[10] = XUtils::ToolBarButton( r, 5, "tool", new BMessage('winl'), B_ONE_STATE_BUTTON, this));
	tool[10]->SetTarget(this);
		
		
	bpm->SetTarget(this);
	bpm_menu->SetTargetForItems(this);
	bpm_menu->SetLabelFromMarked(false);
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
				curpat->SetValue(key2);
				//Window()->PostMessage(new BMessage(POSPAT),this);
				
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
		curpat->SetValue(curpat->GetValue()-1);
		//Window()->PostMessage(new BMessage(SETPAT),this);
		break;
		case B_PAGE_DOWN:
		curpat->SetValue(curpat->GetValue()+1);
		//Window()->PostMessage(new BMessage(SETPAT),this);
		break;
		default:break;
		
	}
	
	if(key[0]>'0' && key[0]<='9'){
	
		int id=key[0]-'1';
		if(id<curSong->getNumberTrack())
		tp->SelectTrack(id);
	
	}
	
		
}
