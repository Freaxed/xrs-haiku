/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "FixedText.h"
#include <string.h>

static BFont* fixed_font=NULL;

#define FAMILY	"ProFont"
#define STYLE	"Regular"
#define SIZE	12

static bool setFixedFont(){
	
	BFont font; 
	font.SetFamilyAndStyle(FAMILY,STYLE);
	
	font_family family;
	font_style style;
	
	font.GetFamilyAndStyle(&family,&style);
	font.SetSize(SIZE);
	
	if(strncmp(family,FAMILY,strlen(family))==0 && strncmp(style,STYLE,strlen(style)) == 0)
	{
			fixed_font=new BFont(font);
			return true;
	}
	return false;
}

FixedText::FixedText(BRect rect,const char* text):
	BStringView(rect,"_fixed",text,B_FOLLOW_NONE,B_WILL_DRAW){
	
	if(fixed_font==NULL && !setFixedFont())
			SetFont(be_plain_font);
	//		debugger("can't find ProFont installed!!");
	
	else
		SetFont(fixed_font);
}

//
