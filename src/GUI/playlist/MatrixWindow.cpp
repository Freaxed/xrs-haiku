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

#include "PlaylistBox.h"

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

	//AddChild(fBox = new PlaylistBox(Bounds()));
	
	WindowManager::Get()->RegisterMe(this,T_PLAYLIST_TITLE);
	LoadConfig();
}

MatrixWindow::~MatrixWindow()
{ 
	SaveConfig();
}


void
MatrixWindow::Reset(Sequence* s, int16 notesPerMeasaure)
{
	if(Lock())
	{
		//fBox->Reset(s, notesPerMeasaure);
		Unlock();
	}
}

bool
MatrixWindow::QuitRequested()
{
	WindowManager::Get()->Hide(this);
	return false;
}

