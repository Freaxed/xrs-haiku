#ifndef _Xed_Utils
#define _Xed_Utils

#include <Application.h>
#include <Resources.h>
#include <InterfaceKit.h>
#include <Path.h>


class XUtils
{
	public:
		static status_t 			CheckIsBank(entry_ref *ref);
		static void	  			CheckMimeType(BApplication*);
		static void	 			 GetBankRef(const char *,entry_ref*);
		
		static status_t			GetDefaultSongRef(entry_ref *rif);
		static void				GetSkinDirectoryPath(BPath* thePath);
		static void				GetBankDirectoryPath(BPath* thePath);
		static void				GetXRSDirectoryEntry(entry_ref *f,const char *dirname);
	
		static BBitmap* 			GetBitmapResource(type_code type,const char* name);
		
		static void				ResetList(BView*);
		static BBitmap*			GetBitmap(int32);
		static BPicture*			GetPicture(int32);
		static BPicture*			MakePicture(BBitmap*,BView*); 			//used only there.
		static BPicture*			MakePictureSkin(const char *,BView*);
		static BBitmap*			MakeBitmapSkin(const char *name);
		static BPictureButton*	ToolBarButton(BRect buttonsize, int32 index, const char *tip, BMessage *msg, uint32 state, BView *view);
		static BPictureButton*	EasyButton(BRect buttonsize, int32 index,BMessage *msg, BView *view);
		
		static BAlert*			ShowIdleAlert(const char* multilineText);
		static void				HideIdleAlert(BAlert*);
		static BBitmap*			CreateBootBitmap(BRect size);


		static	void			FillPresetsMenu(const char* plugname,BMenu* men, uint32);
		static	bool			SavePreset(const char* plugname,const char* presname,BMessage* msg, const char* mime = NULL);
		static	bool			LoadPreset(const char* plugname,const char* presname,BMessage* msg, const char* mime = NULL);
		
		static	void			AddMime(BFile* file, const char* MimeString);
		static	bool			CheckMimeType(BFile* file, const char* MimeString);
private:



};
#endif
