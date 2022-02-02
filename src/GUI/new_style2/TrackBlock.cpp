/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "TrackBlock.h"
#include "TrackList.h"
#include "TrackEnd.h"

#include <Window.h>

TrackBlock::TrackBlock(BRect r, char* name):BView(r,name,  B_FOLLOW_NONE,B_WILL_DRAW) 
{
	SetViewColor(214,219,239);
	expanded=false;
	selected=false;
}

void
TrackBlock::MessageReceived(BMessage* msg)
{
	float c=0;
	switch(msg->what)
	{
	
	case 'rez':
				
		c=getExpansionSize();
		for(int i=0;i<trackend_list.Count();i++)
		{
			TrackEnd *te=trackend_list[i];
			if(te->IsExpanded()) 
				c += te->getExpansionSize();
				
		}
		
		if(IsExpanded()){
			c *=-1;
			UnExpand(c);
			}
		else 
			Expand(c);
		
		((TrackList*)Parent())->Expanded(this,c);
			
	break;
	
	default:
		BView::MessageReceived(msg);
	break;
	
	}	
}

void
TrackBlock::Draw(BRect r)
{	
	BView::Draw(r);
	SetHighColor(255,255,255);
	if(r.Intersects(BRect(0,Bounds().bottom-1,Bounds().right,Bounds().bottom)))
	
		StrokeLine(BPoint(r.left,Bounds().bottom),BPoint(r.right,Bounds().bottom));
	
}

void
TrackBlock::Expand(float delta)
{
	ResizeBy(0,delta);
	expanded=true;
	Invalidate();
	for(int j=0;j<trackend_list.Count();j++)
		trackend_list[j]->Refresh();
		
}
void
TrackBlock::UnExpand(float delta)
{
	ResizeBy(0,delta);
	expanded=false;
	Invalidate();
}
void
TrackBlock::Expanded(TrackEnd* te,float delta)
{
	if(!te) return;
	float c=te->getExpansionSize();
	
	
	((TrackList*)Parent())->Expanded(this,c);
	Expand(c);	
	
	int i=trackend_list.IndexOf(te);
		
	for(int j=i+1;j<trackend_list.Count();j++)
	{
		trackend_list[j]->MoveBy(0,te->getExpansionSize());			
	}
	

}
void
TrackBlock::UnExpanded(TrackEnd* te,float delta)
{
	if(!te) return;
	float c=te->getExpansionSize();
		
	((TrackList*)Parent())->Expanded(this,-1*c);
	Expand(-1*c);		
	
	
	int i=trackend_list.IndexOf(te);
		
	for(int j=i+1;j<trackend_list.Count();j++)
	{
		trackend_list[j]->MoveBy(0,-1*te->getExpansionSize());			
	}
	
}
//.
