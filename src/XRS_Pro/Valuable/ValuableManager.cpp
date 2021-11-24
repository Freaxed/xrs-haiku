/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "ValuableManager.h"
#include "ValuableController.h"
#include "Logger.h"

#include <stdio.h>
#include <String.h>

#define HLOCK(cc)  if(cc->Looper() && cc->LockLooper()){
#define HUNLOCK(cc) cc->UnlockLooper();	}

#define IFX if(false)

ValuableManager::ValuableManager():BLooper("ActiveObserver")
{ 
	messenger = new BMessenger(this);
	Run();
}

ValuableManager*
ValuableManager::Get() 
{
 	static	ValuableManager*	instance = NULL;
	if (instance == NULL) 
			instance = new ValuableManager();
	return instance;
}

		
ValuableManager::~ValuableManager(){
	delete messenger;	
}

bool	
ValuableManager::RegisterValuable(ValuableID id, Valuable* val)
{
	
	printf("RegisterValuable [%s]  %p\n",id.String(), val);
	
	iterator iter = map.Find(id);
			
	if(iter != map.End()) 
	{
		Log(LOG_WARN,"RegisterValuable: Valuable already registered [%s]", id.String());
		return false;
	}
	else
	{
		valuable_list* vlist = new valuable_list;
		vlist->value = val;
		map.Insert(id,vlist);
	}
	
	if(val)
		Log(LOG_INFO,"Registered Valuable [%s]",id.String());
	else
		Log(LOG_INFO,"Registered Valuable [%s] (as NullValuable)",id.String());
			
	return true;
}

bool	
ValuableManager::RegisterValuableView(ValuableID id, ValuableView* val, bool update){

	
	// should be already present!
	iterator iter = map.Find(id);
	
	if ( iter == map.End() ) 
	{
		Log(LOG_WARN,"RegisterValuableView: can't find a Valuable with name %s",id.String());
		return false;
	}
	
	valuable_list *vlist = iter->Value();
	vlist->list.Add(val);
	return true; 

}

float	
ValuableManager::RetriveValue(ValuableID id,int channel){
	
	iterator iter = map.Find(id);
	
	if ( iter == map.End() ) 
		return 0.0;
		
	valuable_list *vlist = iter->Value();
	
	if(vlist->value == NullValuable)
		return 0.0; //NullValuable doesn't support this method.
		
	return vlist->value->GetValue(channel); 

}


ValuableID	
ValuableManager::FindValuableID(Valuable* valuable){

	iterator iter = map.Begin();

	//printf("*\nready to parse for %d  count %ld\n*\n", (int)valuable, map.Count());

	while ( iter != map.End())
	{
		valuable_list* item=iter->Value();
		
		//printf("while %s %d\n",iter->Key().String(), (int)item->value);
		
		if (item->value == valuable)
			return iter->Key();
		
		iter++;
	}	
	return "";
}


void	
ValuableManager::UnRegisterValuable(ValuableID id){

	iterator iter = map.Find(id);
	
	if ( iter == map.End() ) 
		return;
		
	valuable_list *vlist = iter->Value();
	map.Remove(id);
	delete vlist;
		
}

void	
ValuableManager::UnRegisterValuableView(ValuableID id, ValuableView* value)
{	
	iterator iter = map.Find(id);
	
	if ( iter == map.End() ) 
		return;
		
	valuable_list *vlist = iter->Value();
	vlist->list.Remove(value);
}

void	
ValuableManager::DirectSetValuable(ValuableID id, int channel, int32 value)
{
	iterator iter = map.Find(id);
	
	if ( iter == map.End() ) 
		return;
		
	valuable_list *vlist = iter->Value();
	if(vlist->value != NullValuable)
		vlist->value->LowSetValue(channel, value);
}
	
void 
ValuableManager::MessageReceived(BMessage* msg) {
	
	if(msg->what == MSG_CONTROLLER_VALUE)
	{		
		_ValuableChanged(msg);
	}
	else
		BLooper::MessageReceived(msg);
}

void	
ValuableManager::SpreadValue(ValuableID id,int ch,float value){
	
	BMessage* information=new BMessage();
	information->what = MSG_CONTROLLER_VALUE;
	information->AddString("valuable:id",id);
	information->AddFloat("valuable:value",value);	
	information->AddInt16("valuable:value:id",ch);	
	information->AddPointer("valuable:controller",NULL);
	
	PostMessage(information,this);
}


void	
ValuableManager::_ValuableChanged(BMessage* msg){

	ValuableID 	id;
	float 		value;
	void 		*sender=NULL;
	int16		valid;
	int32		bvalue;
		
	msg->FindString("valuable:id",&id);	
	
	iterator iter = map.Find(id);
		
	if (iter == map.End())
	{
		Log(LOG_WARN,"ValuableManager:MSG_VALUABLE_CHANGED can't find the Valuable with name %s",id.String());
		return;
	}	
	
	valuable_list *vlist = iter->Value(); 
		
	msg->what = MSG_VALUABLE_CHANGED;	
		
	msg->FindInt16("valuable:value:id",&valid);
	msg->FindPointer("valuable:controller",&sender);
	
	
	if(msg->FindFloat("valuable:value",&value)!=B_OK)
		msg->AddFloat("valuable:value",0.0);
		
	if(msg->FindInt32("be:value",&bvalue) == B_OK){ //for automatic BControl
		value = (float)bvalue;
		msg->ReplaceFloat("valuable:value",value);
	}
	
	if(vlist->value != NullValuable)
		vlist->value->LowSetValue(valid,(int32)value);
	
	if(vlist->list.Count() == 0)
	{
		Log(LOG_INFO,"ValuableManager:MSG_VALUABLE_CHANGED no registred ValuableView with id %s",id.String());
		return;
	}
	
	Vector<ValuableView*>::Iterator viter = vlist->list.Begin();
	
	while ( viter != vlist->list.End()) 
	{
		ValuableView *vv = *viter;
		if ( vv!=sender && valid == vv->GetChannel())
			_SendMessage(vv,msg);
		viter++;
	}
}

void	
ValuableManager::_SendMessage(ValuableView* view ,BMessage* msg) {
	BHandler *h=view->Handler();
	BMessenger mes(h);
	//#debug
	mes.SendMessage(msg,h);
}



