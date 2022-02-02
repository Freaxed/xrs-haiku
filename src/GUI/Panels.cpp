/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Window.h"
#include "Alert.h"

#define PREFS_H 250
#define PREFS_L 400

#include "Panels.h"
#include <TabView.h>
#include <StringView.h>
#include <Box.h>
#include <Button.h>
#include <FilePanel.h>
#include <Application.h>
#include <Path.h>

#include "StatusBar.h"
#include "RadioButton.h"
#include "Xed_Utils.h"
#include "GlobalDef.h"
#include "Configurator.h"
#include "JFileManager.h"
#include "GfxMsg.h"
#include "Song.h"
#include "TextView.h"
#include "XDigit.h"
#include "BzWindow.h"
#include "locale.h"


extern int 	tracknote_division;
static		entry_ref rif;
static		BTextControl	*txt;

#define BOXSPACE	10.0
#define OFFSET		5.0

void
Panels::showPrefsPanel()
{
	BBox *box,*box2;
	BzWindow *win=new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_FLOATING_WINDOW,0);
	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE);
	
	win->AddChild(box=new BBox(win->Bounds(),"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_PLAIN_BORDER));
	BRect r(box->Bounds());
	r.InsetBy(BOXSPACE,BOXSPACE);
	r.bottom -= BOXSPACE+20;
	box->AddChild(box2=new BBox(r,T_PREFS_TITLE,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_FANCY_BORDER));
	box2->SetLabel(T_PREFS_TITLE);
	r=box2->Bounds();
	
	r.left += OFFSET;
	r.right -= OFFSET;
	r.top=50;
	r.bottom =r.top+20;
	box2->AddChild( 	new BStringView(r,"",T_PREFS_VST_FOLDER,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.left += OFFSET;
	r.right -= 100;
	r.top = r.bottom + 10;
	r.bottom +=20;
	
	box2->AddChild( txt=new BTextControl(r,"vstfolder",NULL,T_PREFS_NOT_SET,B_FOLLOW_NONE,B_WILL_DRAW) );
	

	r.top -= OFFSET;
	r.bottom -= OFFSET;
	r.left=r.right+OFFSET;
	r.right=r.right+=80;
	
	box2->AddChild(new BButton(r,"",T_GEN_SET,new BMessage('Set'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	r=box2->Bounds();
	
	r.left += OFFSET;
	r.right -= OFFSET;
	r.top=150;
	r.bottom =r.top+20;
	
	box2->AddChild(new BStringView(r,"",T_PREFS_RESTART,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r=box2->Frame();
	r.top=r.bottom+5;
	r.bottom =r.top+20;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));
		
	box->AddChild(new BButton(r,"",T_GEN_CANCEL,new BMessage('Canc'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.right = r.left - OFFSET;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));

	box->AddChild(new BButton(r,"",T_GEN_SAVE,new BMessage('Save'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	
	BEntry e;
	BPath p;
	status_t err=Config()->Ref("vstfolder",&rif,"/boot/xeD/");
	
	if(err==B_OK){
	e.SetTo(&rif);
	e.GetPath(&p);
	txt->SetText(p.Path());
	}
	
		
	
	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	win->Show();
	win->RedirectMessages(Panels::msgPrefs,(void*)win);
}
bool
Panels::msgPrefs(BMessage* message,void* cookies)
{
	BzWindow *win=(BzWindow*)cookies;
	/*if(win->Lock()) win->Quit();
	return true;*/
	
	//BMessenger *m;
	BEntry e;
	BPath p;
	
	switch(message->what)
	{
		case 'Set':
			{
			BMessenger *m=new BMessenger(NULL,win);
			BFilePanel *openpanel=new BFilePanel(B_OPEN_PANEL,m,&rif,B_DIRECTORY_NODE,false,NULL,NULL,true,true);
			openpanel->Show();
			}
		break;
		case 'Canc':
		
			win->PostMessage(B_QUIT_REQUESTED);
				
		break;
		case 'Save':
			
			get_ref_for_path(txt->Text(),&rif);			
			Config()->ReplaceRef("vstfolder",&rif);
			win->PostMessage(B_QUIT_REQUESTED);
			
		break;
		case B_REFS_RECEIVED:
		
			if(message->FindRef("refs",&rif)==B_OK)
			{
				e.SetTo(&rif);
				e.GetPath(&p);
				BTextControl *txt=(BTextControl*)win->FindView("vstfolder");
				txt->SetText(p.Path());
			}
		break;
		default:
			return true;
		break;
	}
	
	return true;
	
}


//Setting
void
Panels::showSettings(Song* curSong)
{
	BBox 			*box,*box2;
	XDigit		*a,*b;
	BTextView	*tw;
	BCheckBox	*cb;
	
	BzWindow *win=new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_FLOATING_WINDOW,0);
	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE);
	
	win->AddChild(box=new BBox(win->Bounds(),"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_PLAIN_BORDER));
	BRect r(box->Bounds());
	r.InsetBy(BOXSPACE,BOXSPACE);
	r.bottom -= BOXSPACE+20;
	box->AddChild(box2=new BBox(r,T_SETTINGS_TITLE,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_FANCY_BORDER));
	box2->SetLabel(T_SETTINGS_TITLE);
	
	r=box2->Bounds();
	r.right =r.Width()/2.;
	r.left +=20;
	
	r.top=r.top+20;
	r.bottom =r.top+20;
	
	box2->AddChild(new BStringView(r,"",T_SETTINGS_SONG_DESCRIPTION,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.left=r.right+1;
	r.right *=2;
	r.right -=20;
	box2->AddChild(cb=new BCheckBox(r,"",T_SETTINGS_POPUP_ONLOAD,NULL));
	
	r=box2->Bounds();
	r.InsetBy(20,20);
	r.top +=21;
	r.bottom -=30;
	BRect x;
	x.top=0;
	x.left=0;
	x.right=r.Width();
	x.bottom=r.Height();
	box2->AddChild(tw=new BTextView(r,"",x,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r=box2->Bounds();
	r.right =(r.Width())/2.;
	r.left +=20;
	r.top = r.bottom-39;
	r.bottom=r.top+32;
		
	BBox *sampler= new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW, B_FANCY_BORDER);		
	x=sampler->Bounds();
	x.left=x.right-40;
	x.right=x.left+36;
	x.top=5;
	x.bottom=x.top+22;
	sampler->AddChild(a=new XDigit(x, VID_EMPTY, "sampler?", NULL, 4,32));
	box2->AddChild(sampler);
	x=sampler->Bounds();
	x.InsetBy(4,4);
	x.right -=45;
	sampler->AddChild(new BStringView(x,"set",T_SETTINGS_NOTES_PER_MEASURE,B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.left=r.right+5;
	r.right *=2;
	r.right -=20;
	sampler=new BBox(r ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,B_WILL_DRAW, B_FANCY_BORDER);
	x=sampler->Bounds();
	x.left=x.right-40;
	x.right=x.left+36;
	x.top=5;
	x.bottom=x.top+22;
	sampler->AddChild(b=new XDigit(x, VID_EMPTY,"sampler2?", NULL, 2,8));
	box2->AddChild(sampler);
	x=sampler->Bounds();
	x.InsetBy(4,4);
	x.right -=45;
	sampler->AddChild(new BStringView(x,"set",T_SETTINGS_BEAT_DIVISION,B_FOLLOW_NONE,B_WILL_DRAW));
	r=box2->Frame();
	r.top=r.bottom+5;
	r.bottom =r.top+20;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));
		
	box->AddChild(new BButton(r,"",T_GEN_CANCEL,new BMessage('Canc'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.right = r.left - OFFSET;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));

	box->AddChild(new BButton(r,"set",T_GEN_OK,new BMessage('Ok'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	tw->SetStylable(true);
	tw->SetText(curSong->getDescription());
	cb->SetValue(curSong->popupdesc);
	
	a->UpdateValue((int32)curSong->getNumberNotes(), true);
	b->UpdateValue((int32)tracknote_division, true);
	
	BMessage *out=new BMessage();
	out->AddPointer("win",(void*)win);
	out->AddPointer("song",(void*)curSong);
	out->AddPointer("a",(void*)a);
	out->AddPointer("b",(void*)b);
	out->AddPointer("tw",(void*)tw);
	out->AddPointer("cb",(void*)cb);
	
	
	
	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	win->Show();
	win->RedirectMessages(Panels::msgSettings,(void*)out);

	
}
bool
Panels::msgSettings(BMessage* message,void* cookies)
{
	BzWindow *win;
	BMessage *x=((BMessage*)cookies);
	x->FindPointer("win",(void**)&win);

	
	switch(message->what)
	{
		
		case 'Canc':
		
			win->PostMessage(B_QUIT_REQUESTED);
				
		break;
		case 'Ok':
			{
				Song *curSong;
				x->FindPointer("song",(void**)&curSong);
				XDigit *a,*b;
				x->FindPointer("a",(void**)&a);
				x->FindPointer("b",(void**)&b);
				BTextView	*tw;
				BCheckBox	*cb;
				x->FindPointer("tw",(void**)&tw);
				x->FindPointer("cb",(void**)&cb);
					
				curSong->setDescription(tw->Text());
				curSong->setNumberNotes(a->GetValue());
				curSong->popupdesc=(bool)cb->Value();
				tracknote_division=(int)b->GetValue();
				be_app->PostMessage(new BMessage(SONG_RESET));
				win->PostMessage(B_QUIT_REQUESTED);
			}
		break;
		
		default:
			return true;
		break;
	}
	
	return true;
	
}

void
Panels::showExport(int k,int m,JFileManager* jfm)
{/*
	BBox *box,*box2,*box3;
	BTabView	*tabview;
	BTab		*tab;
	XDigit		*a;
	BRadioButton	*radio,*radio2,*radio4,*radio6;
	
	BzWindow *win=new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_FLOATING_WINDOW,0);
	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE);
	
	win->AddChild(box=new BBox(win->Bounds(),"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_PLAIN_BORDER));
	BRect r(box->Bounds());
	r.InsetBy(20,20);
	r.bottom -=30;
	box->AddChild(box2=new BBox(r,T_EXP_TITLE,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_FANCY_BORDER));
	box2->SetLabel(T_EXP_TITLE);
	
	r=box2->Bounds();
	r.InsetBy(10,20);
	//r.right=r.left+100;
	//'r.bottom=r.top+100;
	box2->AddChild(tabview=new BTabView(r, "tab_view"));
	//box2->AddChild(box3=new BBox(r,"Export Song",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_FANCY_BORDER));
	//box3->SetLabel("Section");
	r=tabview->Bounds();
	r.bottom -=tabview->TabHeight();
	
	//#ifdef ZETAr
//		box3=new BBox(r,T_EXP_TITLE,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_NO_BORDER);
//		tab=new BStringTab(BString(T_EXP_SECTION),box3);
//		tabview->AddTab(tab);
//	#else
		tab=new BTab();
		tabview->AddTab(box3=new BBox(r,"Export Song",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_NO_BORDER),tab);
		tab->SetLabel(T_EXP_SECTION);
	//#endif
	
	
	
	r=box3->Bounds();
	r.InsetBy(20,20);
	r.right =+ 160;
	r.bottom=r.top+32;
	box3->AddChild(radio=new BRadioButton(r,"radio1",T_EXP_SINGLE_MEASURE,NULL));
	r.left=r.right+5;
	r.right=r.left+36;
	r.bottom=r.top+22;
	box3->AddChild(a=new XDigit(r,NULL,NULL,1,m));
	r=box3->Bounds();
	r.InsetBy(20,20);
	//r.right =+ 180;
	r.top+=32;
	r.bottom=r.top+32;
	box3->AddChild(radio2=new BRadioButton(r,"radio2",T_EXP_ALL_SONG,NULL));
	
	radio->SetValue(1);
	
	r=tabview->Bounds();
	r.bottom -=tabview->TabHeight();
	
	//#ifdef ZETAr
//		box3=new BBox(r,"Export Song",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_NO_BORDER);
//		tab=new BStringTab(BString(T_EXP_FORMAT),box3);
//		tabview->AddTab(tab);
//	#else
		tab=new BTab();
		tabview->AddTab(box3=new BBox(r,"Export Song",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_NO_BORDER),tab);
		tab->SetLabel(T_EXP_FORMAT);
	//#endif
	

	r=box3->Bounds();
	r.InsetBy(20,20);
	//r.right += 160;
	r.bottom=r.top+32;
	box3->AddChild(radio=new BRadioButton(r,"radio3","Wave File (16bit stereo)",NULL));
	r=box3->Bounds();
	r.InsetBy(20,20);
	//r.right =+ 180;
	r.top+=32;
	r.bottom=r.top+32;
	box3->AddChild(radio4=new BRadioButton(r,"radio4","Aiff File (32bit stereo)",NULL));
	radio->SetValue(1);
	
	r=tabview->Bounds();
	r.bottom -=tabview->TabHeight();
	
	//#ifdef ZETAr
//		box3=new BBox(r,"Export Song",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_NO_BORDER);
//		tab=new BStringTab(BString(T_EXP_MIXER),box3);
//		tabview->AddTab(tab);
//	#else
		tab=new BTab();
		tabview->AddTab(box3=new BBox(r,"Export Song",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_NO_BORDER),tab);
		tab->SetLabel(T_EXP_MIXER);
	//#endif
	
		
	r=box3->Bounds();
	r.InsetBy(20,20);
	//r.right =+ 160;
	r.bottom=r.top+32;
	box3->AddChild(radio=new BRadioButton(r,"radio5",T_EXP_MASTER_LINE,NULL));
	r=box3->Bounds();
	r.InsetBy(20,20);
	//r.right =+ 180;
	r.top+=32;
	r.bottom=r.top+32;
	box3->AddChild(radio6=new BRadioButton(r,"radio6",T_EXP_FX_LINES,NULL));
	radio->SetValue(1);

	r=box2->Frame();
	r.top=r.bottom+5;
	r.bottom =r.top+20;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));


	box->AddChild(new BButton(r,"set",T_GEN_CANCEL,new BMessage('Canc'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	r.right = r.left - OFFSET;
	r.left= r.right - (be_plain_font->StringWidth(T_GEN_CANCEL) + 	(OFFSET * 2));

	box->AddChild(new BButton(r,"set",T_GEN_EXPORT,new BMessage('Expt'),B_FOLLOW_NONE,B_WILL_DRAW));
	
	a->SetValue(k);
	
	
	//tabview->Select(0);

	
	BMessage *out=new BMessage();
	out->AddPointer("win",(void*)win);
	out->AddPointer("a",(void*)a);
	out->AddPointer("jfm",(void*)jfm);
	out->AddPointer("radio2",(void*)radio2);
	out->AddPointer("radio4",(void*)radio4);
	out->AddPointer("radio6",(void*)radio6);
	
	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	win->Show();
	win->RedirectMessages(Panels::msgExport,(void*)out);
	*/
}
bool
Panels::msgExport(BMessage* message,void* cookies)
{
	BzWindow *win;
	BMessage *x=((BMessage*)cookies);
	x->FindPointer("win",(void**)&win);

	
	switch(message->what)
	{
		
		case 'Canc':
		
			win->PostMessage(B_QUIT_REQUESTED);
				
		break;
		case 'Expt':
			{

				XDigit *a;
				x->FindPointer("a",(void**)&a);
				JFileManager	*jfm;
				x->FindPointer("jfm",(void**)&jfm);
				
				BMessage *info=new BMessage('expt');
				
				BRadioButton *rb;
				x->FindPointer("radio2",(void**)&rb);
				info->AddBool("playmode",(bool)rb->Value());
				
				x->FindPointer("radio4",(void**)&rb);
				
				info->AddInt16("format",(int16)rb->Value());
				
				
				x->FindPointer("radio6",(void**)&rb);
				
				
				info->AddInt16("lines",(int16)rb->Value());
				info->AddInt16("position",a->GetValue()-1);
				
				jfm->ExportWave(info);
			}
		break;
		
		default:
			return true;
		break;
	}
	
	return true;
}
void
Panels::showErrors(BMessage* list)
{
	if(list==NULL || list->IsEmpty()) return;
	 
	BBox 			*box;
	BTextView		*tw;

	
	BzWindow *win=new BzWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_FLOATING_WINDOW,0);
	win->SetLook(B_MODAL_WINDOW_LOOK);
	win->SetFeel(B_MODAL_APP_WINDOW_FEEL);
	win->SetFlags(B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE);
	
	win->AddChild(box=new BBox(win->Bounds(),"",B_FOLLOW_ALL_SIDES,B_WILL_DRAW,B_PLAIN_BORDER));
	
	BRect r(box->Bounds());
	r.InsetBy(20,20);
	r.bottom -=30;
	
	BRect x;
	x.top=0;
	x.left=0;
	x.right=r.Width();
	x.bottom=r.Height();
	box->AddChild(tw=new BTextView(r,"set",x,B_FOLLOW_NONE,B_WILL_DRAW));
	
	
	
	r.top=r.bottom+10;
	r.bottom =r.top+20;
	r.left= r.right-60;
	
	box->AddChild(new BButton(r,"set","Oh!",new BMessage(B_QUIT_REQUESTED),B_FOLLOW_NONE,B_WILL_DRAW));
	
	
	tw->SetStylable(true);
	
	BString error_str;
	int i=0;
	while(list->FindString("error",i,&error_str)==B_OK)
	{
		
		tw->Insert(error_str.String());
		i++;
	}

	win->MoveTo(BAlert::AlertPosition(PREFS_L,PREFS_H));
	win->Show();
	//win->RedirectMessages(Panels::msgErrors,(void*)NULL);

	
}
/*
bool
Panels::msgErrors(BMessage* message,void* cookies)
{
	BzWindow *win;
	//BMessage *x=((BMessage*)cookies);
	//x->FindPointer("win",(void**)&win);

	
	switch(message->what)
	{
				
		default:
			return true;
		break;
	}
	
	return true;
}*/
