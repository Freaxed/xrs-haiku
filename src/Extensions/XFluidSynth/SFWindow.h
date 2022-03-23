
#ifndef _SFWindow_H
#define _SFWindow_H
 
#include <Window.h>
#include <fluidlite.h>

class APot;
class BCheckBox;
class ADigit;
class BMenuField;
class BStringView;

class SFWindow:public BWindow
{
 public:
				SFWindow();
		void 	MessageReceived(BMessage* message);
		bool 	QuitRequested();
		void	Reset();
		
 private:
		void	InitGUI();
		
		int		chorus_type;
		
		APot*	pot_reverb_roomsize;
		APot* 	pot_reverb_damping;
		APot*	pot_reverb_width;
		APot*	pot_reverb_level;
						
		APot*	pot_chorus_depth; //ms
		APot*	pot_chorus_level;
		APot*	pot_chorus_speed; //Hz
		
		APot*	pot_gain;
		
		ADigit*		digit_chorus_n;
		BMenuField*	field_chorus_type;
				
		BCheckBox*	ck_chorus;
		BCheckBox*	ck_reverb;
		
		BStringView*	str_name;
		
		APot*	createCanvas(BRect r, const char* txt, int32 mess, BView*, int min = 0, int max = 100);
		void	set_reverb();
		void	set_chorus();
		
		
};

#endif
