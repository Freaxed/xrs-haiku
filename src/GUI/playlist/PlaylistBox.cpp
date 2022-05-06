/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "PlaylistBox.h"
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include "locale.h"
#include "GfxMsg.h"
#include "maxcount.h"

#include "XMPoz.h"
#include "XMName.h"
#include "XMatrix.h"
#include "MeasureManager.h"
#include "Sequence.h"
#include "MainWindow.h"
#include "JMessages.h"
#include "TextControlFloater.h"

#define XBOX			18
#define	Y_COUNT			NUM_PATTERN
#define	LOOP_ENABLE		'lope'
#define	RENAME_PATTERN	'renp'

PlaylistBox::PlaylistBox(BRect _r) : BBox(_r, "PlaylistBox", B_FOLLOW_ALL_SIDES, B_PLAIN_BORDER)
{
    //Menu Bar
	BMenuBar* menuBar = new BMenuBar(BRect(0,0,100,20), "Menu Bar",B_FOLLOW_LEFT,B_ITEMS_IN_ROW,false);
	
	
	float menuH, menuW;
	menuBar->GetPreferredSize(&menuW, &menuH);
	
	if(menuH>21) menuBar->ResizeBy(0,menuH-21);
	else menuH=21;
	
	
	
	BMenu *menuMea;
	menuBar->AddItem(menuMea=new BMenu(T_PLAYLIST_MEASURES));
	menuMea->AddItem(new BMenuItem(T_GEN_ADD,new BMessage(ADD_PATTERN)));
	menuMea->AddItem(new BMenuItem(T_GEN_REMOVE,new BMessage(REMOVE_PATTERN)));
	menuMea->AddSeparatorItem();
	menuMea->AddItem(new BMenuItem(T_GEN_RENAME,new BMessage('rena')));

	AddChild(menuBar);

	//Position
	AddChild( tt = new XMPoz(BRect(101, menuH-19, _r.right - B_V_SCROLL_BAR_WIDTH, menuH-1)));
	//Matrix
	BRect r=Bounds();
	r.left=101;
	r.top=menuH;
	
	r.right -=B_V_SCROLL_BAR_WIDTH;
	r.bottom -=B_V_SCROLL_BAR_WIDTH;
	//MeasureName
	the_n=new XMName(BRect(0,menuH,100,r.bottom));
	AddChild(the_n);


	the_m = new XMatrix(r, the_n, tt);
	AddChild(scroll=new BScrollView("w2",the_m,B_FOLLOW_ALL_SIDES,B_WILL_DRAW,true,true));
	
	scroll_bar=scroll->ScrollBar(B_HORIZONTAL);
	scroll->SetViewColor(ViewColor());	

	AddChild(en=new BCheckBox(BRect(0,r.bottom+1,101,r.bottom+B_V_SCROLL_BAR_WIDTH+1),"",T_PLAYLIST_LOOP_POINTS,new BMessage(LOOP_ENABLE),B_FOLLOW_BOTTOM));
	
	
	the_m->MakeFocus(true);	
	scroll_bar->SetRange(0,X_COUNT*XBOX-the_m->Bounds().Width());
}

void
PlaylistBox::FrameResized(float w, float h)
{
	scroll_bar->SetRange(0,X_COUNT*XBOX-the_m->Bounds().Width());
    BBox::FrameResized(w, h);
}

void
PlaylistBox::MessageReceived(BMessage *m)
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
			BBox::MessageReceived(m); 
		break;
	}
}

void
PlaylistBox::Reset(Sequence* s, int16 notesPerMeasaure)
{
    seq=s;	
    en->SetValue(s->loop_enable);
    tt->Reset(s, notesPerMeasaure);
    the_n->Reset(s);
    the_m->Reset(s);
}