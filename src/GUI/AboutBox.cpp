/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "AboutBox.h"
#include "Xed_Utils.h"
#include "version.h"
#include "locale.h"
#include "URLView.h"

#include <malloc.h>
#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <Window.h>
#include <String.h>
#include <Roster.h>

#define PREFS_H 204
#define PREFS_L 300

#define CLOSE_NOW	'clos'

AboutBox::~AboutBox(){
	delete shot;
}

void
AboutBox::WindowActivated(bool on)
{
	BWindow::WindowActivated(on);
	if(!on) 
		return;
	
	BRect r(Bounds());
	r.OffsetBy(centered_point);
	
	XUtils::CreateBootBitmap(&r,&shot, true);
	
	w->SetViewBitmap(shot);
	MoveTo(centered_point);

	w->Invalidate();
}

void
AboutBox::MessageReceived(BMessage* m)
{
	if(m->what == CLOSE_NOW)
	{
		 PostMessage(new BMessage(B_QUIT_REQUESTED));
	}
	else
		BWindow::MessageReceived(m);
	
}
AboutBox::AboutBox(bool mode):BWindow(BRect(0,0,PREFS_L,PREFS_H),"", B_MODAL_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE)
{
	float larg = PREFS_L;
	
	if(mode) 
	{
		larg += 100;
		ResizeBy(100,0);
	}
	
	MoveTo(-2*larg,-2*PREFS_H);
	big_mode=mode;
	centered_point=BAlert::AlertPosition(larg,PREFS_H);

	if(!mode) 
		SetLook(B_NO_BORDER_WINDOW_LOOK);
	
	BView *sporca=new BView(BRect(0,0,10,10),"",B_FOLLOW_NONE,B_WILL_DRAW);
	AddChild(sporca);
	
	XUtils::ResetList(sporca);
	RemoveChild(sporca);
	
	BRect r(Frame());
	
	w=new BView(Bounds(),"",B_FOLLOW_NONE,B_WILL_DRAW);
	AddChild(w);
	
	r=Bounds();
	
	r.top=r.bottom+1;
	
	float k=0;
	if(mode) 
		k=150;
		
	r.bottom+=k;
	ResizeBy(-1,k-1);
	
	BBox *but=new BBox(r,"",B_FOLLOW_ALL,B_WILL_DRAW);
	
		
	if(mode){
	

	
	BButton *ok=new BButton(BRect(300,100,300+61,100+30),"","..ok?",new BMessage(CLOSE_NOW));
	ok->SetFontSize(12);
	but->AddChild(ok);
	text=NULL;
	
	
	
	URLView *email = new URLView( BRect( 20, 35, 299, 50 ), "Author ", "Andrea Anzani", "" );
	email->SetHoverEnabled( true );
	email->AddAttribute( "META:name", "Andrea Anzani" );
	email->AddAttribute( "META:nickname", "xeD" );
	email->AddAttribute( "META:url", "https://github.com/Freaxed/xrs-haiku" );
	email->AddAttribute( "META:country", "Italy" );
	
	
	but->AddChild( new BStringView(BRect(10,10,200,35),"",T_DIALOG_WRITTEN_BY)); 
	but->AddChild( email );
	but->AddChild(new URLView(BRect( 20,50,200,65),"","GitHub Page","https://github.com/Freaxed/xrs-haiku"));
	
	BStringView *sv;
	but->AddChild( sv = new BStringView(BRect(10,70,299,85),"","FluidLite library (LGPL) by www.iiwu.org")); 
	sv->SetFontSize(10);
	but->AddChild( sv = new BStringView(BRect(10,88,299,103),"","VST Plug-In Technology by Steinberg")); 
	sv->SetFontSize(10);
	
	BString version;
	version << "Version: " << VERSION_NUMBER << "    Build: " << __DATE__ << "   [" << CODENAME << "]" << "[Haiku version]";
	
	but->AddChild(sv =new BStringView(BRect(10,50+80,380,50+95),"",version.String()));
	sv->SetFontSize(10);
	
	}
	 else
	{
		but->AddChild( text=new BStringView(BRect(0,0,r.Width(),20),"",T_DIALOG_LOADING)); 
	}

	AddChild(but);
	
}
void
AboutBox::setText(const char*t)
{
//	if( text!=NULL )	
	if(Lock())
	{
		text->SetText(t); 
		Unlock();
 	}
}

