/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#ifndef	_VSTMANAGER_H_
#define	_VSTMANAGER_H_

class	BList;
class	BMenu;
class	BFile;
class 	VSTItem;

#include    <List.h>
#include	"VSTList.h"

class VstManager
{
	public:
			 
			~VstManager();
		
		static	VstManager*	Get();
		
		VSTItem*	CreateVst(VSTPlugin* fromList);
		void		DeleteVst(VSTItem* plug);

		bool		SavePreset(VSTItem* plug,const char*name,BMessage* msg);
		bool		LoadPreset(VSTItem* plug,const char*name,BMessage* msg);
		
		void		FillPresetsMenu(VSTItem* plug,BMenu*,uint32 msg);
		void		setPresetsPath(VSTItem* plug);
		
		const BList*		InstrumentVst() { return &fInstList; }
		const BList*		EffectVst()     { return &fFxList;   }
		
			
	private:
		
		void		GetInstrumentVst(BList*);
		void		GetEffectVst(BList*);
		
					VstManager();
		VSTList*	fVstList;
		BList		fFxList;
		BList		fInstList;
};

#endif
