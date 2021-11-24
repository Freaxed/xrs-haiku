#include 	"JMDrumPanel.h"
#include	"JMDrumTrack.h"
#include	"Xed_Utils.h"
#include   	"XHost.h"
#include	"Drum.h"
#include  	"MachineInterface.h"
#include	"TextControlFloater.h"
#include	"JMDrumTrackBoost.h"
#include	<MenuBar.h>
#include	<StringView.h>

#define TWEAK 'TWEK'

#define	X_SAVE_PRESET		'xspt'
#define	X_LOAD_PRESET		'xlpt'
#define	X_SAVE_PRESET_OK 	'xspo'
#define	X_SAVE_LOST			'xslo'

#include "ScrollView.h"

extern const CMachineInfo* 	info;
extern JMDrumTrackBoost* jm_booster;
BMenu*		presetz;
BMenu*		men;

#define	PRESET_REVISION		1 //NB: >>0!
#define PRESET_DIRECTORY	"JMDrumExtension"

extern void ParameterTweak(int par, int val,int* Vals,DrumPars *globalpar);
extern BString DescribeValue(int const param, int const value);

JMDrumPanel::JMDrumPanel():PlugPanel(){


		//menu bar
		BRect r(Bounds());
		BMenuBar	*barra=new BMenuBar(r,"barra");
		men=new BMenu(T_VSTWIN_PRESETS);
		
		presetz=new BMenu(T_VSTWIN_USER);
		XUtils::FillPresetsMenu(PRESET_DIRECTORY,presetz,X_LOAD_PRESET);
		
		men->AddItem(presetz);
		men->AddSeparatorItem();
		men->AddItem(new BMenuItem(T_MENU_SAVE_AS,new BMessage(X_SAVE_PRESET)));
		
	
		barra->AddItem(men);
		
		
		barra->ResizeToPreferred();
		AddChild(barra);
		//

		r.top = barra->Bounds().bottom+1;
		 
		r.right  -= 16;
		r.bottom -= 5;
		BMessage *msg;
		 
			
		 BView *box=new BView(r,"names",B_FOLLOW_NONE,B_WILL_DRAW);
		 
		 r=Bounds();
		
		 r.bottom=40;
		 r.right -=20;
		 
		for(int i=0;i<16;i++)	sl[i]=NULL;
			
		for(int i=0;i<16;i++)
		{
		 if(i==6 || i==9) continue; 
		 
		 msg=new BMessage(TWEAK);
		 msg->AddInt16("id",i);
		 
		 box->AddChild(sl[i]=new BSlider(r,"ciao",info->Parameters[i]->Name,NULL,info->Parameters[i]->MinValue,info->Parameters[i]->MaxValue));
		 sl[i]->SetModificationMessage(msg);
		 sl[i]->SetValue(info->Parameters[i]->DefValue);
		 sl[i]->SetFontSize(10);
		 BRect frame(sl[i]->Bounds());
		 frame.left = frame.right*0.6; //(frame.right + frame.left) / 2;
		 frame.top  +=5;
		 frame.bottom = frame.top + 11;

		 txt[i] = new BStringView (frame, "value", "bo");
		 
		 sl[i]->AddChild (txt[i]);
		 txt[i]->SetAlignment (B_ALIGN_RIGHT);
		 txt[i]->SetFontSize(10);
		 display=DescribeValue(i,info->Parameters[i]->DefValue);
		 txt[i]->SetText(display.String());
		
		  
		 r.OffsetBy(0,41);
		
		 
		}
		//box->ResizeBy(0,100);
		
		BScrollView *sw=new BScrollView("pars",box,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,false,true,B_PLAIN_BORDER);
		BScrollBar *b=sw->ScrollBar(B_VERTICAL);
		b->SetRange(0,sw->Bounds().Width()+r.Width()+barra->Bounds().bottom+1);
		AddChild( sw );
		 
		//
		//sw->FrameResized(sw->Bounds().Width(),sw->Bounds().Height());
		
		myTrack=NULL;
}

void
JMDrumPanel::Reset(Track* tr){
	if(tr){
	
		SetTitle(tr->getName());
		
		if(Window()->Lock()){
		 myTrack=(JMDrumTrack*)tr;
		
		  
		  
		for(int i=0;i<16;i++)
		{
			if(sl[i]) sl[i]->SetValue(myTrack->Vals[i]);
			display=DescribeValue(i,myTrack->Vals[i]);
			if(sl[i]) txt[i]->SetText(display.String());
		
		}
		
		
		 Window()->Unlock();
		}
	}
	else
		myTrack=NULL;
	PlugPanel::Reset(tr);
	
	
};
void
JMDrumPanel::AttachedToWindow()
{
	for(int i=0;i<16;i++)
		if(sl[i])  sl[i]->SetTarget(this);
		
	men->SetTargetForItems(this);
	presetz->SetTargetForItems(this);
	
	PlugPanel::AttachedToWindow();
	
}
void
JMDrumPanel::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case TWEAK:
		 if(!myTrack) return;
		 int id;
		 id=msg->FindInt16("id");
		 ParameterTweak(id,msg->FindInt32("be:value"),myTrack->Vals,&myTrack->values);
		 display=DescribeValue(id,msg->FindInt32("be:value"));
		if(sl[id]) txt[id]->SetText(display.String());
		 //pro->SetLabel(display.String());
		break;
		case X_SAVE_PRESET:
		{
			BRect frame(Bounds());
			frame=ConvertToScreen(Bounds());
			float x=frame.left+((frame.right-frame.left)/2.0)-100;
			float	y=frame.top+50;
			XHost::Get()->SendMessage(X_MainWindowDeactivate,0);
			
			TextControlFloater *tf=new TextControlFloater(BRect(BRect(x,y,x+200,y+20)),B_ALIGN_LEFT,be_plain_font,"",this,new BMessage(X_SAVE_PRESET_OK),new BMessage(X_SAVE_LOST));
			tf->SetLook(B_FLOATING_WINDOW_LOOK);
			tf->SetTitle(T_VSTWIN_NAME);
		}
		break;
		case X_SAVE_LOST:
			XHost::Get()->SendMessage(X_MainWindowActivate,0);
		break;
		case X_SAVE_PRESET_OK:
		{
			XHost::Get()->SendMessage(X_MainWindowActivate,0);
			
			if(!myTrack || !jm_booster) return;
			
			BString nome;
			msg->FindString("_value",&nome);
			if(nome.Length()<=0) return; 
			 BMessage	set;
			 jm_booster->SaveTrackSettings((Track*)myTrack,&set);
			
			set.AddString("name",nome.String());
			set.AddInt16("revision",PRESET_REVISION);
			
			if(XUtils::SavePreset(PRESET_DIRECTORY,nome.String(),&set)){
				BMenuItem* ni;
				presetz->AddItem(ni=new BMenuItem(nome.String(),new BMessage(X_LOAD_PRESET)));
				ni->SetTarget(this);
			}
		}
		break;
		case X_LOAD_PRESET:
		{
			XHost::Get()->SendMessage(X_MainWindowActivate,0);
			
			if(!myTrack || !jm_booster) return;
			
			int pos=msg->FindInt32("index");
			BMessage	set;
			BMenuItem	*item=presetz->ItemAt(pos);
			
			if(XUtils::LoadPreset(PRESET_DIRECTORY,item->Label(),&set)){
				if(set.FindInt16("revision")==PRESET_REVISION){
					jm_booster->LoadTrackSettings((Track*)myTrack,&set);
					Reset((Track*)myTrack);
				}
			}	
		}
		break;
		default:
		 PlugPanel::MessageReceived(msg);
		break;
	}
}
