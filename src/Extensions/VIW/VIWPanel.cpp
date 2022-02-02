/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include 	"VIWPanel.h"
#include	"VIWTrack.h"
#include	"VSTInstrument.h"
#include	"Xed_Utils.h"
#include	"PlugWindow.h"
#include	<StringView.h>
#include 	"XHost.h"
#include 	"viw_locale.h"
#include	"VstManager.h"

#define	SET_VSTI	'stvs'
#define	REMOVE_VSTI	'revs'
#define	SHOW_VSTI_WINDOW	'sviw'

VIWPanel::VIWPanel(const BList* vst):PlugPanel(){

	myTrack=NULL;
	sampler= new BBox(BRect(8,18,172,50) ,"toolbar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE_JUMP, B_FANCY_BORDER);		
	
	menu=new BMenu(T_VIW_NOVST);

	menu->AddItem(new BMenuItem(T_VIW_NOVST,new BMessage(REMOVE_VSTI)));
	BMessage*	info;
	for(int16 i=0;i<vst->CountItems();i++){
			VSTPlugin*	stru = (VSTPlugin*)vst->ItemAt(i);
			info = new BMessage(SET_VSTI);
			info->AddInt16("id",i);
			menu->AddItem(new BMenuItem(stru->EffectName(),info));
	}
	BRect r(sampler->Bounds());
	r.InsetBy(4,4);
	r.right -=40;
	
	field=new BMenuField(r,"",T_VIW_NOVST,menu);
	sampler->AddChild(field);
	
	AddChild(sampler);
	
	r.right +=40;
	r.left  =r.right-24;
	
	but=r;
	
	vst_list=vst;
}

void
VIWPanel::Reset(Track* tr){
	if(tr){
	
		SetTitle(tr->getName());
		if(Window()->Lock()){
		myTrack=(VIWTrack*)tr;
		if(myTrack->getWin())
				myTrack->getWin()->SetTitle(tr->getName());
		
		if(myTrack->getViw())
			menu->Superitem()->SetLabel(myTrack->getViw()->EffectName());
				else
			menu->Superitem()->SetLabel(T_VIW_NOVST);
			
		Window()->Unlock();
		}
	}
	else	myTrack=NULL;
	
	PlugPanel::Reset(tr);
	
	
};
void
VIWPanel::AttachedToWindow()
{
	sampler->AddChild(brez=XUtils::ToolBarButton( but, 8, "_winW", new BMessage(SHOW_VSTI_WINDOW), B_ONE_STATE_BUTTON, sampler));
	brez->SetTarget(this);
				
	PlugPanel::AttachedToWindow();
	menu->SetTargetForItems(this);
	field->SetDivider(0);

}
void
VIWPanel::MessageReceived(BMessage* msg)
{
	if(myTrack==NULL)
	{
				PlugPanel::MessageReceived(msg);
				return;
	}
	
	if(msg->what==SET_VSTI){
		int k=msg->FindInt16("id");
		VSTPlugin	*stru=(VSTPlugin*)vst_list->ItemAt(k);

		myTrack->LoadVSTi(stru);
		
		if(myTrack->getViw())
			menu->Superitem()->SetLabel(myTrack->getViw()->EffectName());
		else
			menu->Superitem()->SetLabel(T_VIW_NOVST);
		
	}
	else
	if(msg->what==REMOVE_VSTI){
		
		
		
		myTrack->LoadVSTi(NULL);
		menu->Superitem()->SetLabel(T_VIW_NOVST);
		
	}
	else
		if(msg->what==SHOW_VSTI_WINDOW){
		

		if(myTrack->getWin())
			XHost::Get()->SendMessage(X_Switch,(void*)myTrack->getWin());
		
		
	}
	else
	PlugPanel::MessageReceived(msg);
}
