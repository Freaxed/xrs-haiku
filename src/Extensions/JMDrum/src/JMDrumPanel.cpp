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
#include	<LayoutBuilder.h>
#include	<GroupLayout.h>
#include	<ScrollView.h>
#include 	<Autolock.h>

#define TWEAK 'TWEK'

#define	X_SAVE_PRESET		'xspt'
#define	X_LOAD_PRESET		'xlpt'
#define	X_SAVE_PRESET_OK 	'xspo'
#define	X_SAVE_LOST			'xslo'

extern const CMachineInfo* 	info;
extern JMDrumTrackBoost* jm_booster;


#define	PRESET_REVISION		1 //NB: >>0!
#define PRESET_DIRECTORY	"JMDrumExtension"

extern void ParameterTweak(int par, int val,int* Vals,DrumPars *globalpar);
extern BString DescribeValue(int const param, int const value);

JMDrumPanel::JMDrumPanel():PlugPanel(){

	// Create menu bar
	BMenuBar *barra = new BMenuBar("menu_bar");
	men = new BMenu(T_VSTWIN_PRESETS);

	presetz = new BMenu(T_VSTWIN_USER);
	XUtils::FillPresetsMenu(PRESET_DIRECTORY, presetz, X_LOAD_PRESET);

	men->AddItem(presetz);
	men->AddSeparatorItem();
	men->AddItem(new BMenuItem(T_MENU_SAVE_AS, new BMessage(X_SAVE_PRESET)));
	barra->AddItem(men);

	// Create container for sliders
	BView *box = new BView("names", B_WILL_DRAW);
	BGroupLayout *boxLayout = new BGroupLayout(B_VERTICAL, 0);
	box->SetLayout(boxLayout);
	box->SetExplicitMinSize(BSize(180, B_SIZE_UNSET));
	box->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));

	// Create sliders
	for(int i = 0; i < 16; i++) {

		if(i == 6 || i == 9) {
			sl[i] = NULL;
			continue;
		}

		BMessage *msg = new BMessage(TWEAK);
		msg->AddInt16("id", i);

		sl[i] = new BSlider("slider", info->Parameters[i]->Name, nullptr,
			info->Parameters[i]->MinValue, info->Parameters[i]->MaxValue, B_HORIZONTAL);
		sl[i]->SetModificationMessage(msg);
		sl[i]->SetValue(info->Parameters[i]->DefValue);


		txt[i] = new BStringView("value", "");
		txt[i]->SetAlignment(B_ALIGN_RIGHT);
		txt[i]->SetText(DescribeValue(i, info->Parameters[i]->DefValue).String());

		boxLayout->AddView(sl[i]);
		boxLayout->AddView(txt[i]);
	}

	// Create scroll view
	BScrollView *sw = new BScrollView("pars", box, B_WILL_DRAW, false, true);

	// Build layout
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(barra)
		.Add(sw)
	.End();

	//SetExplicitMinSize(BSize(180, 230));
	//SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));

	myTrack = NULL;
}

void JMDrumPanel::ResetToTrack(Track *tr) {

  if (tr) {

    SetTitle(tr->getName());

    BAutolock lock(Window());
    myTrack = dynamic_cast<JMDrumTrack *>(tr);
    if (!myTrack)
      return;

    for (int i = 0; i < 16; i++) {
      if (sl[i] != nullptr) {
        sl[i]->SetValue(myTrack->Vals[i]);
        txt[i]->SetText(DescribeValue(i, myTrack->Vals[i]).String());
      }
    }

  } else {
    myTrack = NULL;
  }

  PlugPanel::ResetToTrack(tr);
};
void
JMDrumPanel::AttachedToWindow()
{
	for(int i=0;i<16;i++) {
		if(sl[i] != nullptr)
			sl[i]->SetTarget(this);
	}

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
		{
          if (!myTrack)
            return;
          int id = msg->GetInt16("id", 0);
          ParameterTweak(id, msg->FindInt32("be:value"), myTrack->Vals,
                         &myTrack->values);
          if (sl[id])
            txt[id]->SetText(DescribeValue(id, msg->FindInt32("be:value")).String());
          break;
		 }
        case X_SAVE_PRESET: {
          BRect frame(Bounds());
          frame = ConvertToScreen(Bounds());
          float x = frame.left + ((frame.right - frame.left) / 2.0) - 100;
          float y = frame.top + 50;
          XHost::Get()->SendMessage(X_MainWindowDeactivate, 0);

          TextControlFloater *tf = new TextControlFloater(
              BRect(BRect(x, y, x + 200, y + 20)), B_ALIGN_LEFT, be_plain_font,
              "", this, new BMessage(X_SAVE_PRESET_OK),
              new BMessage(X_SAVE_LOST));
          tf->SetLook(B_FLOATING_WINDOW_LOOK);
          tf->SetTitle(T_VSTWIN_NAME);
        } break;
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
					ResetToTrack((Track*)myTrack);
				}
			}
		}
		break;
		default:
		 PlugPanel::MessageReceived(msg);
		break;
	}
}
