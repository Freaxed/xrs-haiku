/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Xed_Utils.h"
#include "Activity.h"
#include "Utils.h" //libfunky
#include "Log.h"

// defines da spostare/capire
#define		B_VOLPAD		0;
#define		B_VOLPADOFF 	1;
#define		B_PANPAD		2;
#define		B_TRACKNAMEON	3;
#define		B_TRACKNAMEOFF	4;
#define		B_LINE			5;
#define		P_FILTERON	0;
#define		P_FILTEROFF	1;
//


#include <Application.h>
#include <Roster.h>
#include <StorageKit.h>
#include <InterfaceKit.h>
#include <TranslationUtils.h>
#include <List.h>
#include <stdio.h>
#include <string.h>
#include <TypeConstants.h>
#include <Screen.h>
#include <GroupLayout.h>
#include "Log.h"

//don't move.
static	BList		*p_list;	
static	BList		*b_list;
static 	BResources  *rsrc;
		
void
XUtils::ResetList(BView* view)
{
	 BPath path;
	 XUtils::GetSkinDirectoryPath(&path);
	 path.Append("default");
	 rsrc = new BResources(new BFile(path.Path(),B_READ_ONLY));
	
	
	if (p_list != NULL && b_list != NULL) 
		return;
		
	p_list = new BList(0);
	b_list = new BList(0);
	
	if (view != NULL)
	{
		p_list->AddItem((void*)MakePictureSkin("plus",view));		//0
		p_list->AddItem((void*)MakePictureSkin("minus",view));		//1
		p_list->AddItem((void*)MakePictureSkin("PlayOff",view));	//2
		p_list->AddItem((void*)MakePictureSkin("PlayOn",view));		//3
		p_list->AddItem((void*)MakePictureSkin("AllOff",view));		//4
		p_list->AddItem((void*)MakePictureSkin("AllOn",view));		//5
		p_list->AddItem((void*)MakePictureSkin("max",view));		//6				
		p_list->AddItem((void*)MakePictureSkin("max_off",view));	//7
		p_list->AddItem((void*)MakePictureSkin("par_tool_off",view));	//8				
		p_list->AddItem((void*)MakePictureSkin("par_tool",view));	//9
		p_list->AddItem((void*)MakePictureSkin("donate.gif",view));	//10
	
	}
	
	BBitmap *nil=NULL;
	
	
	b_list->AddItem((void*)MakeBitmapSkin("volpad"));			//0
	b_list->AddItem((void*)MakeBitmapSkin("volpadoff"));			//1
	b_list->AddItem((void*)MakeBitmapSkin("panpad"));			//2
	b_list->AddItem((void*)MakeBitmapSkin("text.png"));//3
	b_list->AddItem((void*)MakeBitmapSkin("Slider.png"));//4
	b_list->AddItem((void*)MakeBitmapSkin("Line"));			//5
	b_list->AddItem((void*)nil);//6
	b_list->AddItem((void*)MakeBitmapSkin("Morbido"));			//7
	b_list->AddItem((void*)MakeBitmapSkin("piano6"));			//8
	b_list->AddItem((void*)MakeBitmapSkin("ButtonRedOn"));		//9
	b_list->AddItem((void*)MakeBitmapSkin("ButtonRedOff"));		//10
	b_list->AddItem((void*)MakeBitmapSkin("ButtonBluOn"));		//11
	b_list->AddItem((void*)MakeBitmapSkin("ButtonBluOff"));		//12
	b_list->AddItem((void*)MakeBitmapSkin("PatBase"));			//13
	b_list->AddItem((void*)MakeBitmapSkin("mydigit"));			//14
	b_list->AddItem((void*)nil);//15
	b_list->AddItem((void*)nil);//16
	b_list->AddItem((void*)nil);//17
	b_list->AddItem((void*)MakeBitmapSkin("BackSample"));		//18
	b_list->AddItem((void*)MakeBitmapSkin("ToolIcons"));	//19
	b_list->AddItem((void*)MakeBitmapSkin("EmptyButton"));//20
	b_list->AddItem((void*)MakeBitmapSkin("MeterOff"));//21
	b_list->AddItem((void*)MakeBitmapSkin("MeterOn"));//22
	b_list->AddItem((void*)MakeBitmapSkin("positions"));//23
	b_list->AddItem((void*)MakeBitmapSkin("ToolbarPot"));//24	
	b_list->AddItem((void*)nil);//25
	b_list->AddItem((void*)nil);//26	
	b_list->AddItem((void*)MakeBitmapSkin("TrackNameMask"));//27		
}



BBitmap*
XUtils::GetBitmapResource(type_code type,const char* name)
{
	size_t len = 0;
	
	if (rsrc == NULL)
	{
		BPath path;
		XUtils::GetSkinDirectoryPath(&path);
		path.Append("default");
		rsrc = new BResources(new BFile(path.Path(),B_READ_ONLY));
	}
	
	const void *data = rsrc->LoadResource(type, name, &len);

	if (data == NULL) 
	{
		LogError("Can't load skin element : %s\n",name);
		return NULL;
	}
	
	BMemoryIO stream(data, len);
	
	stream.Seek(0, SEEK_SET);
	BBitmap* out=BTranslationUtils::GetBitmap(&stream);
	
	return out;
}


BPicture*
XUtils::GetPicture(int32 index)
{

	if(index >= p_list->CountItems()) return NULL;
	else 
		return (BPicture*)(p_list->ItemAt(index)); 	
}

BBitmap*
XUtils::GetBitmap(int32 index)
{	
	BBitmap *bit = (BBitmap*)(b_list->ItemAt(index)); 	
	
	if( bit == NULL ) {
		LogError("XUtils::GetBitmap for index %d found nothing!", index);
		bit = new BBitmap(BRect(0,0,1,1),B_GRAY1);
	}
	return bit;
}

BPictureButton*
XUtils::ToolBarButton(BRect buttonsize, int32 index, const char *tip, BMessage *msg, uint32 state, BView *view)
{
	BBitmap *icons = new BBitmap(GetBitmap(19)); 
	BBitmap *up    = new BBitmap(GetBitmap(20));
	BBitmap *dis   = new BBitmap(up);

	
	uint8 *in = (uint8*)icons->Bits() +index*25*4;
	uint8 *out = (uint8*)up->Bits();
	uint8 *dout = (uint8*)dis->Bits();
	
	uint32 add = up->BytesPerRow() + 104;
	uint8 d;
	
	for(int32 y=0; y<22; y++){
		for(int32 x=0; x<25; x++){
			if (in[0]==0 && in[1]==255 && in[2]==0){
				in+=4;
				out+=4;
				dout+=4;
			}else{
				d = (uint8)(in[0]*0.15+in[1]*0.25+in[2]*0.1) + 100;
				out[add] = in[0];
				out[add+1] = in[1];
				out[add+2] = in[2];
				*out++ = *in++;
				*out++ = *in++;
				*out++ = *in++;
				out++;	in++;
				dout[add] = d;
				dout[add+1] = d;
				dout[add+2] = d;
				*dout++ = d;
				*dout++ = d;
				*dout++ = d;
				dout++;
			}
		}
		in += icons->BytesPerRow() -25*4;
		out += up->BytesPerRow() -25*4;
		dout += up->BytesPerRow() -25*4;
	}
	
	BPicture *Up,*Down, *DUp, *DDown;

	view->BeginPicture(new BPicture);
	view->DrawBitmap(dis, BRect(0,0,24,22), BRect(0,0,24,22) );
	DUp = view->EndPicture();
   
	view->BeginPicture(new BPicture);
	view->DrawBitmap(dis, BRect(25,0,49,22), BRect(0,0,24,22) );
	DDown = view->EndPicture();

	view->BeginPicture(new BPicture);
	view->DrawBitmap(up, BRect(0,0,24,22), BRect(0,0,24,22) );
	Up = view->EndPicture();
   
	view->BeginPicture(new BPicture);
	view->DrawBitmap(up, BRect(25,0,49,22), BRect(0,0,24,22) );
	Down = view->EndPicture();
	view->Sync();

	BPictureButton *PBut = new BPictureButton(buttonsize, NULL, Up, Down, msg, state, B_FOLLOW_NONE, B_WILL_DRAW);
		
	PBut->SetDisabledOff(DUp);
	PBut->SetDisabledOn(DDown);

	
	delete Up;
	delete Down;
	delete DDown;
	delete DUp;
	
	return PBut;
}
BPictureButton*
XUtils::EasyButton(BRect buttonsize, int32 index,BMessage *msg, BView *view)
{

	const BBitmap *icons = LoadIcon("SmallIcons");
	const BBitmap *up    = LoadIcon("EmptyButtonSmall");
	
	BPicture *Up,*Down;

	view->BeginPicture(new BPicture);
	 view->SetDrawingMode(B_OP_ALPHA);
	 view->DrawBitmap(up, BRect(0,0,18,17), BRect(0,0,18,17) );
	 view->DrawBitmap(icons, BRect(19*index,0,19*index+18,17), BRect(0,0,18,17) );
	Up = view->EndPicture();
   
	view->BeginPicture(new BPicture);
	 view->SetDrawingMode(B_OP_ALPHA);
	 view->DrawBitmap(up, BRect(19,0,37,17), BRect(0,0,18,17) );
	 view->DrawBitmap(icons, BRect(19*index,0,19*index+18,17), BRect(1,1,19,18) );
	Down = view->EndPicture();

	BPictureButton *PBut = new BPictureButton(buttonsize, NULL, Up, Down, msg, 0, B_FOLLOW_NONE, B_WILL_DRAW);

	delete Up;
	delete Down;
	return PBut;
}

void
XUtils::CreateBootBitmap(BRect* z, BBitmap** img, bool big)
{
	BBitmap *image=*img;
	BBitmap	*shot;
	
	if(!big){
		BScreen s(B_MAIN_SCREEN_ID);
		s.GetBitmap(&shot,false,z);
	}
	
	image = new BBitmap(*z, B_RGBA32, true);
	BView	*vista=new BView(BRect(0,0,z->right-z->left,z->bottom-z->top),"",B_FOLLOW_ALL,B_WILL_DRAW);
	
	image->AddChild(vista);
	
	if(image->Lock())
	{	
		if(!big)
		vista->DrawBitmap(shot,BPoint(0,0));
		else
		{
			vista->SetHighColor(255,255,255,255);
			vista->FillRect(vista->Bounds());
		}
		vista->SetDrawingMode(B_OP_ALPHA);
		vista->DrawBitmap(LoadIcon("logo.gif"),BPoint(0,0));
		vista->Sync();
		image->Unlock();
	}
	
	
	image->RemoveChild(vista);
	
	*img=image;
	
	if(!big)
		delete shot;
	delete vista;
}

BPicture*
XUtils::MakePicture(BBitmap* bit ,BView* vie)
{
	BPicture *on;
	BRect rect(bit->Bounds());
	BView *tempView = new BView( rect, "temp", B_FOLLOW_NONE, B_WILL_DRAW );
	
	vie->AddChild(tempView);
	
	tempView->BeginPicture(new BPicture); 

	if(bit)
   		tempView->DrawBitmap(bit);

   	on = tempView->EndPicture();
   	vie->RemoveChild(tempView);
   	delete tempView;
   	
   	return on;
}
void
XUtils::GetSkinDirectoryPath(BPath* thePath)
{
	app_info theInfo;
	BEntry 	theEntry;
	BDirectory theDirectory;
	status_t status = be_app->GetAppInfo(&theInfo);
	if(status == B_OK) {
		
		if(theEntry.SetTo(&theInfo.ref) == B_OK
		&& theEntry.GetParent(&theDirectory) == B_OK) {
			thePath->SetTo(&theDirectory,"Skin");
			thePath->InitCheck();
			
		}
	}  
}
void
XUtils::GetBankDirectoryPath(BPath* thePath)
{
	app_info theInfo;
	BEntry 	theEntry;
	BDirectory theDirectory;
	status_t status = be_app->GetAppInfo(&theInfo);
	if(status == B_OK) {
		
		if(theEntry.SetTo(&theInfo.ref) == B_OK
		&& theEntry.GetParent(&theDirectory) == B_OK) {
			thePath->SetTo(&theDirectory,"Banks");
			thePath->InitCheck();
			
		}
	}  
}
void
XUtils::GetXRSDirectoryEntry(entry_ref *f,const char *dirname)
{
	app_info theInfo;
	BEntry 	theEntry;
	BPath	thePath;
	BDirectory theDirectory;
	status_t status = be_app->GetAppInfo(&theInfo);
	if(status == B_OK) {
		
		if(theEntry.SetTo(&theInfo.ref) == B_OK
		&& theEntry.GetParent(&theDirectory) == B_OK) {
			thePath.SetTo(&theDirectory,dirname);
			thePath.InitCheck();
			get_ref_for_path(thePath.Path(),f);
			
		}
	}  
}

void
XUtils::GetBankRef(const char* name,entry_ref*rif)
{
	BPath 		thePath;

	GetBankDirectoryPath(&thePath);
	thePath.Append(name);
	get_ref_for_path(thePath.Path(),rif);
}

BPicture*
XUtils::MakePictureSkin(const char *name,BView *w)
{
	const BBitmap* bitmap = LoadIcon(name);
	
	if (!bitmap) 
		printf("Can't load skin element : %s\n",name);
	
	return MakePicture((BBitmap*)bitmap,w);
}

BBitmap*
XUtils::MakeBitmapSkin(const char *name)
{
	const BBitmap* bitmap = LoadIcon(name);
	
	if(!bitmap) 
		printf("Can't load skin element : %s\n",name);
	
	return (BBitmap*)bitmap;
}
status_t
XUtils::CheckIsBank(entry_ref *ref)
{
	BNodeInfo	info;
	BNode		node(ref);
	
	char	t[B_MIME_TYPE_LENGTH];
	
	status_t err;
	
	err=node.InitCheck();
	if(err!=B_OK) return err;
	
	err=info.SetTo(&node);
	if(err!=B_OK) return err;
	
	err=info.GetType(t);
	if(err!=B_OK) return err;
	
	if(strncmp(t,"audio/XRS-Bank",14)!=0) return B_ERROR;
	
	return B_OK;
}

status_t
XUtils::GetDefaultSongRef(entry_ref *rif)
{
	BPath 		thePath;
	
	
	GetXRSDirectoryEntry(rif,"Songs");
	
	BEntry e(rif,true);
	
	thePath.SetTo(&e);
	
	thePath.Append("DefaultSong");
	
	get_ref_for_path(thePath.Path(),rif);
	
	
	e.SetTo(rif);
	e.GetRef(rif);
	if(e.Exists()) return B_OK;
			else return B_ERROR;
}
void
XUtils::CheckMimeType(BApplication *app)
{
	BMimeType m("audio/XRS-File");
	m.InitCheck();
	if(!m.IsInstalled())
		{
			m.SetPreferredApp("application/x-vnd.xeD.XRS");
			m.Install();
			printf("MimeType installed!\n");
		}

}
BAlert *
XUtils::ShowIdleAlert(const char* multilineText)
{
	BAlert * al = new BAlert("Wait", multilineText, "...");
	
	BGroupLayout* group = dynamic_cast<BGroupLayout*>(al->GetLayout());
	BLayout* lay = dynamic_cast<BLayout*>(group->ItemAt(group->CountItems()-1));	
	Activity *act = NULL;
	lay->AddView(act = new Activity("_activity_"));
	act->SetExplicitMinSize(BSize(10, 20));
	
	al->ButtonAt(0)->Hide();
	
	act->Start();
	al->Go(NULL);
	
	return al;
}
void
XUtils::HideIdleAlert(BAlert *al)
{
	if(al->Lock()) al->Quit();
}

void
XUtils::FillPresetsMenu(const char* name, BMenu* men, unsigned long msg){

	BDirectory	xdir;
	BPath		xpath;
	status_t	err;
	LogDebug("Filling presets menu for [%s]", name);
	
	find_directory (B_USER_SETTINGS_DIRECTORY, &xpath, true);
	xpath.Append ("XRSPresets");
	xpath.Append (name);
	
	err = xdir.SetTo(xpath.Path());
	if(err == B_ENTRY_NOT_FOUND) {
		LogError("No presets directory found [%s]", xpath.Path());
		return;
	}
	
	BEntry e;
	entry_ref rif;

	while(xdir.GetNextEntry(&e,true)==B_OK)
	{
		e.InitCheck();		
		if(e.IsFile()){
			e.GetRef(&rif);
			LogDebug("Found preset for [%s]. name [%s]", name, rif.name);
			men->AddItem(new BMenuItem(rif.name,new BMessage(msg)));
		}
		
	}
}

bool			
XUtils::SavePreset(const char* plugname, const char* presname, BMessage* msg, const char* mime){

	BDirectory	xdir;
	BPath		xpath;
	status_t	err;
	
	find_directory (B_USER_SETTINGS_DIRECTORY, &xpath, true);
	xpath.Append ("XRSPresets");
	xpath.Append (plugname);
	
	err=xdir.SetTo(xpath.Path());
	
	if(err == B_ENTRY_NOT_FOUND){
		create_directory(xpath.Path(), 0777);
		xdir.SetTo(xpath.Path());
	} else if (err != B_OK) {
		LogError("Error getting directory: [%s]", xpath.Path());
		return false;
	}
	
	BFile	*file=new BFile();
	xpath.Append(presname);
	LogInfo("Saving preset for [%s] at path [%s]\n", plugname, xpath.Path());
	

	BEntry controllo_esiste(xpath.Path());
	if(controllo_esiste.Exists()) {	
		LogError ("Preset already present! at path [%s]\n", xpath.Path());
		delete file;
		return false;
	}
	
	err = file->SetTo(xpath.Path(),B_WRITE_ONLY|B_CREATE_FILE);
	
	if(err!=B_OK) {
	
		LogError ("Error in creating the file [%s] err %d\n", xpath.Path(), err);
		delete file;
		return false;
	}
	
	msg->Flatten(file);
	
	if (mime)
		AddMime(file, mime);
	delete file;
	return true;
	

}

bool			
XUtils::LoadPreset(const char* plugname,const char* presname,BMessage* msg, const char* mime){

	BDirectory	xdir;
	BPath		xpath;
	status_t	err;
	
	find_directory (B_USER_SETTINGS_DIRECTORY, &xpath, true);
	xpath.Append ("XRSPresets");
	xpath.Append (plugname);
	
	err=xdir.SetTo(xpath.Path());
	if(err==B_ENTRY_NOT_FOUND) return false;
	
	BFile	*file=new BFile();
	xpath.Append(presname);
	printf("LOAD full filename %s\n",xpath.Path());
	err=file->SetTo(xpath.Path(),B_READ_ONLY);
	
	if(err!=B_OK) {
		delete file;
		return false;
	}
	
	if(mime && !CheckMimeType(file, mime)) {
		printf ("wrong file type %d\n",err);
		delete file;
		return false;
	}
		
	msg->Unflatten(file);
	
	delete file;
	return true;
	
}
void
XUtils::AddMime(BFile* file, const char* MimeString)
{
	BNodeInfo	info;
	info.SetTo(file);
	info.SetType(MimeString);
}

bool
XUtils::CheckMimeType(BFile* file, const char* MimeString)
{
	BNodeInfo	info;
	status_t 	err;
	err=info.SetTo(file);
	char mime[B_MIME_TYPE_LENGTH];
	err=info.GetType(mime);

	return (strcmp(mime,MimeString) == 0);
}

		
