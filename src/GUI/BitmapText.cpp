#include "BitmapText.h"
#include "string.h"
#include "Xed_Utils.h"

#define W_LETTER 5.
#define H_LETTER 6.


BitmapText::BitmapText(BRect rect,const char* text):
	BView(rect,NULL,B_FOLLOW_NONE,B_WILL_DRAW){
	
	if(text) {
	
		//debugger("BT");
		
		i_len= strlen(text);
		fText=(char*)malloc(i_len);
		strcpy(fText,text);
		
	
			
		//B_ALIGN_CENTER
		
		float p_len 	= strlen(fText) * W_LETTER;
				
		p_start	= (rect.Width()/2.0) - (p_len/2.0);
		p_end	= p_start + p_len -1;
	
		rects = new BRect[i_len];
		
		for(int i=0;i<i_len;i++){
		 
		 char let = fText[i];
		 
		 int  x_pos = 0 ;
		 int  y_pos = 0 ;
		 
		 if(  let >= 'a' &&  let <= 'z' )	
		 	x_pos = let - 'a';
		 else	
		 if(  let >= 'A' &&  let <= 'Z' )	
		 	x_pos = let - 'A';	
		 
		 rects[i].Set( x_pos * W_LETTER,y_pos * H_LETTER, x_pos * W_LETTER + W_LETTER - 1 ,y_pos * H_LETTER + H_LETTER -1 );
		 rects[i].PrintToStream();
		}
		
		ResizeTo(rect.Width(),H_LETTER);
	}
	sourceBitmap =XUtils::GetBitmap(3);
	if(!sourceBitmap) debugger("cant find bitmap 3!");
	//SetViewColor(B_TRANSPARENT_COLOR);

}

void
BitmapText::Draw(BRect rect){	
	
	if(rect.Intersects( BRect(p_start,0,p_end,H_LETTER-1) )) {
		for(int i=0;i<i_len;i++)
			DrawBitmapAsync(sourceBitmap,rects[i],BRect(p_start+i*W_LETTER,0,p_start+i*W_LETTER+W_LETTER-1,H_LETTER-1));	
	}
}

//
