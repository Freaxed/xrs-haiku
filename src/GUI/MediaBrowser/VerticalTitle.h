#ifndef _VERTICALTITLE_H_
#define _VERTICALTITLE_H_

#include <Box.h>
#include <String.h>
#include <Window.h>
#include <Message.h>


class VerticalTitle: public BBox
{
	public:
			 VerticalTitle(BRect r,const char *txt,BMessage *info);
	
	virtual void 	AttachedToWindow();
	virtual void 	Draw(BRect);
	virtual void	MouseUp(BPoint);
	virtual void	MouseDown(BPoint){ wait_click=true;};	
		  void 	SetStringPos(BPoint x);
		  void	SetTextColor(rgb_color cc){ text_color=cc;};
		  void	SetColor(rgb_color cc);
		  void	SetText(const char* t){text=t; Invalidate();}	
	private:
		BString text;
		BPoint	spec;
		rgb_color	color;
		rgb_color	text_color;
		BMessage	*msg;
		bool	wait_click;
		
};

#endif
//-
