/*
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "XrsWindow.h"
#include "Configurator.h"	

void
XrsWindow::SaveConfig() {		
			
	BString temp(fName);
	temp += "Xpos";
	Config()->PutFloatKey(temp.String(),Frame().left);
	
	temp = fName;
	temp += "Ypos";
	Config()->PutFloatKey(temp.String(),Frame().top);
	
	temp = fName;
	temp += "Xstop";
	Config()->PutFloatKey(temp.String(),Frame().right-Frame().left);
	
	temp = fName;
	temp += "Ystop";
	Config()->PutFloatKey(temp.String(),Frame().bottom-Frame().top);
}
		
void
XrsWindow::LoadConfig(BRect* aframe){
			
	BRect frame;
	if (aframe) 
		frame = *aframe;
	else
		frame = fFrame;
		
	BString temp=fName;
	BString temp2=fName;
	temp+="Xpos";
	temp2+="Ypos";
	MoveTo(Config()->FloatKey(temp.String(),frame.left),Config()->FloatKey(temp2.String(),frame.top));
	
	temp=temp2=fName;
	temp+="Xstop";
	temp2+="Ystop";
	ResizeTo(Config()->FloatKey(temp.String(),frame.Width()),Config()->FloatKey(temp2.String(),frame.Height()));

}
		
void
XrsWindow::SetName(const char* name){ 
	fName = name; 
}
