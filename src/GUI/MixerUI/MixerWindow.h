/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_MIXERWINDOW_H_
#define	_MIXERWINDOW_H_

#include  "XrsWindow.h"

class 	VerticalTitle;
class	BListView;
class	VSTItem;
class	BPictureButton;
typedef	void OutputLine;

#define	VERTICAL_CLICK	'verc'
#define	RESET_MIXER		'resm'

class BzWindow;
class BListView;

class MixerWindow : public XrsWindow
{
	public:
					static MixerWindow*	Get();
					
								~MixerWindow();
				void 	MessageReceived(BMessage *);
				bool	QuitRequested();
				void 	Refresh();	
							
				void	CreateVstWindow(VSTItem* vst,int pos);
				void	DeleteVstWindow(int pos);
				
				void	SaveSettings(BMessage*	data);
				void	LoadSettings(BMessage*	data);
			
	private:		
						MixerWindow();
				void	_postValue(int x);
				void	Select(int i);
				void	VSTSelect(int i);
				void	LoadVST(int k,OutputLine*,int i);
				
				int		FindVST(const char*);
				
				bool 	SetVST(BMessage *msg);
				
				// deprecated!
				int		FindVSTOLD(const char*);
				void	Reset();
				
				
				
				VerticalTitle	*strv[8];
				VerticalTitle	*check[8];
								
				BListView		*list;
				BPictureButton	*brez;
				BButton			*ok;
				bool			expanded;
				float			small,big;
				BzWindow		*vst_win;
				
				
};				

#endif
//--
