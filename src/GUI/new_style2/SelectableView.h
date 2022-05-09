/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _SELECTABLE_VIEW_H_
#define _SELECTABLE_VIEW_H_

#include <View.h>
#include "Log.h"


class SelectableView : public BView
{
    public:
                    SelectableView(BRect r) : BView(r, "selectable_view", B_FOLLOW_NONE, B_WILL_DRAW) 
                                            {
                                                  SetSelected (false); 
                                            }

        	virtual void	SetSelected(bool s, bool invalidate = true) 
                    {                  
                        fSelected = s;
                        rgb_color color =  fSelected ? rgb_color{255,227,153} : rgb_color{214,219,239};
                        
                        for (int i=0;i<CountChildren();i++) {
                            ChildAt(i)->SetViewColor(color);
                            if (invalidate)  
                                ChildAt(i)->Invalidate();
                        }

                        SetViewColor(color);
                        if (invalidate) 
                            Invalidate();
                    }

			bool	IsSelected() {return fSelected; }
    private:
            bool        fSelected;

};
#endif //_SELECTABLE_VIEW_H_
