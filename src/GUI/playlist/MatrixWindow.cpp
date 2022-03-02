/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "MatrixWindow.h"
#include "XMatrix.h"
#include "XMName.h"
#include "XMPoz.h"
#include "JMessages.h"
#include "TextControlFloater.h"
#include "maxcount.h"
#include "GfxMsg.h"
#include "Sequence.h"
#include "MainWindow.h"
#include "MeasureManager.h"
#include "WindowManager.h"
#include "locale.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Button.h>
#include <Box.h>
#include <Alert.h>
#include <ScrollView.h>
#include <Message.h>


#define XBOX			18
#define	Y_COUNT			NUM_PATTERN
#define	LOOP_ENABLE		'lope'
#define	RENAME_PATTERN	'renp'

MatrixWindow*
MatrixWindow::Get()
{
	static	MatrixWindow*	instance = NULL;
	if (instance == NULL) 
			instance = new MatrixWindow();
	return instance;
}


MatrixWindow::MatrixWindow() :
	XrsWindow(BRect(250,30,250+452,30+Y_COUNT*XBOX+XBOX+14+14),T_GEN_PLAYLIST, B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS|B_OUTLINE_RESIZE|B_WILL_ACCEPT_FIRST_CLICK|B_AVOID_FOCUS)
{


	SetName("playlist_");
	
	//Panel	
	BBox *panel=new BBox(Bounds(),"",B_FOLLOW_ALL_SIDES,B_PLAIN_BORDER);
/**/	AddChild(panel);
	
	//Menu Bar
	BMenuBar* 
	menuBar = new BMenuBar(BRect(0,0,100,20), "Menu Bar",B_FOLLOW_LEFT,B_ITEMS_IN_ROW,false);
	
	
	float menuH, menuW;
	menuBar->GetPreferredSize(&menuW,&menuH);
	
	//printf("MenuH %f\n",menuH);
	
	if(menuH>21) menuBar->ResizeBy(0,menuH-21);
	else menuH=21;
	
	
	
	BMenu *menuMea;
	menuBar->AddItem(menuMea=new BMenu(T_PLAYLIST_MEASURES));
	menuMea->AddItem(new BMenuItem(T_GEN_ADD,new BMessage(ADD_PATTERN)));
	menuMea->AddItem(new BMenuItem(T_GEN_REMOVE,new BMessage(REMOVE_PATTERN)));
	menuMea->AddSeparatorItem();
	menuMea->AddItem(new BMenuItem(T_GEN_RENAME,new BMessage('rena')));

/**/	panel->AddChild(menuBar);
	//Position
/**/	panel->AddChild(tt=new XMPoz(BRect(101,menuH-19,452-14,menuH-1)));

	//Matrix
	BRect r=panel->Bounds();
	r.left=101;
	r.top=menuH;
	
	r.right -=B_V_SCROLL_BAR_WIDTH;
	r.bottom -=B_V_SCROLL_BAR_WIDTH;
	//MeasureName
	the_n=new XMName(BRect(0,menuH,100,r.bottom));
/**/	panel->AddChild(the_n);

	the_m=new XMatrix(r,the_n,tt);
/**/	panel->AddChild(scroll=new BScrollView("w2",the_m,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,true,true));
	
	scroll_bar=scroll->ScrollBar(B_HORIZONTAL);
	scroll->SetViewColor(panel->ViewColor());


	

	//buttons
	//panel->AddChild(new BButton(BRect(1,Y_COUNT*XBOX+B_V_SCROLL_BAR_WIDTH+1+2,49,Y_COUNT*XBOX+B_V_SCROLL_BAR_WIDTH+1+15),"Add","Add",new BMessage(ADD_PATTERN),B_FOLLOW_BOTTOM));
	//panel->AddChild(new BButton(BRect(51,Y_COUNT*XBOX+B_V_SCROLL_BAR_WIDTH+1+2,99,Y_COUNT*XBOX+B_V_SCROLL_BAR_WIDTH+1+15),"Add","Del",new BMessage(REMOVE_PATTERN),B_FOLLOW_BOTTOM));
/**/	panel->AddChild(en=new BCheckBox(BRect(0,r.bottom+1,101,r.bottom+B_V_SCROLL_BAR_WIDTH+1),"",T_PLAYLIST_LOOP_POINTS,new BMessage(LOOP_ENABLE),B_FOLLOW_BOTTOM));
	
	
	the_m->MakeFocus(true);	
	scroll_bar->SetRange(0,X_COUNT*XBOX-the_m->Bounds().Width());
	
	WindowManager::Get()->RegisterMe(this,T_PLAYLIST_TITLE);
	
	LoadConfig();
}

MatrixWindow::~MatrixWindow()
{ 
	SaveConfig();
}

void
MatrixWindow::FrameResized(float,float)
{
	scroll_bar->SetRange(0,X_COUNT*XBOX-the_m->Bounds().Width());
}


void
MatrixWindow::MessageReceived(BMessage *m)
{
	switch(m->what)
	{
		case 'rena':
			the_n->_MessForRect(MeasureManager::Get()->GetCurrentPattern());
		break;
		case ADD_PATTERN:
		case REMOVE_PATTERN:
			MainWindow::Get()->PostMessage(m);
		break;
		case LOOP_ENABLE:
			seq->loop_enable=(bool)en->Value();
			tt->Invalidate();
		break;
		
		case 'ttty':
		{
		
		BPoint f,t;
		
		m->FindPoint("from", &f);
		m->FindPoint("to", &t);
				
		/*  Disabling KeyBoard for MainWindow*/
		
		if(MainWindow::Get()->Lock()){		
			MainWindow::Get()->WindowActivated(false);
			MainWindow::Get()->Unlock();
		} 
		
		BMessage *mx=new BMessage(JMSG_NAME_SET);
		mx->AddInt16("pos",m->FindInt16("pos"));
		
		new TextControlFloater(BRect(f,t),B_ALIGN_LEFT,be_plain_font,m->FindString("name"),this,mx,new BMessage(JMSG_NAME_NOTSET));
		
		}
		break;
		
		case JMSG_NAME_SET:
			
			const char* name;
			m->FindString("_value", &name);
			
						
			seq->SetMeasureName(name,m->FindInt16("pos"));
			
			if(MainWindow::Get()->Lock()){
				MainWindow::Get()->WindowActivated(true);
				MainWindow::Get()->Unlock();
			}
		
		break;
		
		case JMSG_NAME_NOTSET:
			if(MainWindow::Get()->Lock()){
				MainWindow::Get()->WindowActivated(true);
				MainWindow::Get()->Unlock();
			}
		break;
		
		default:
			BWindow::MessageReceived(m); 
		break;
	}

}


void
MatrixWindow::Reset(Sequence* s, int16 notesPerMeasaure)
{
	if(Lock())
	{
		seq=s;	
		en->SetValue(s->loop_enable);
		tt->Reset(s, notesPerMeasaure);
		the_n->Reset(s);
		the_m->Reset(s);
		Unlock();
	}
}

bool
MatrixWindow::QuitRequested()
{
	WindowManager::Get()->Hide(this);
	return false;
}

