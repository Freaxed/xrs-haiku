/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef _AChannelSlider_
#define _AChannelSlider_


#include <Window.h>
#include <Slider.h>

class AChannelSlider: public BSlider
{
public:
		   AChannelSlider(			BRect rect,
									const char * name,
									const char * label,
									BMessage * msg,int min,int max,
									orientation o):
									
									BSlider(
											rect,
											name,
											label,
											msg,min,max,o){};		

};

#endif
//.
