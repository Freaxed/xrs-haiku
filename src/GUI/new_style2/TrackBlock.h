/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _TRACKBLOCK_H_
#define _TRACKBLOCK_H_

#include <View.h>
#include <Button.h>

#include "VectorNew.h"

class TrackEnd;

class TrackBlock : public BView
{
	public:
			     		 	TrackBlock(BRect r,const char* name);
	 		virtual void    MessageReceived(BMessage*);
	 		virtual void	Draw(BRect r);
	 		bool			IsExpanded(){ return expanded;}
	 		
	 			
	 		virtual void		Expand(float);
	 		virtual void		UnExpand(float);
	 		void				Expanded(TrackEnd*,float);
	 		void				UnExpanded(TrackEnd*,float);
	 		virtual float		getExpansionSize() { return 24;};
	 		
	 		// experimental
	 		
	 		virtual void				SetSelected(bool sel){selected=sel;}
	 									
	 		
	private:
			bool expanded;
			bool selected;
			
			void				_expanded(TrackEnd*, float, int);
			
	protected:

			VectorNew<TrackEnd*>	 trackend_list;
			
};
#endif
