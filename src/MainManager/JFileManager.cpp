/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */


#include "JFileManager.h"
#include "filemaker_msg.h"

#include <stdio.h>
#include <Entry.h>
#include <CheckBox.h>
#include <Message.h>
#include <Messenger.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SymLink.h>
#include <FindDirectory.h>
#include <FilePanel.h>
#include <Roster.h>
#include <Application.h>
#include <Path.h>

#include <Alert.h>
#include <Button.h>
#include <MediaTrack.h>
#include <MediaFile.h>

#include "SJFilter.h"

#include  "Song.h"
#include "Pattern.h"
#include "Note.h"

#include "locale.h"

#include "Xed_Utils.h"
#include 	"Sequence.h"
#include	"MixerWindow.h"
#include 	"TrackManager.h"

#include	"version.h"

#define 	CLEAR_TRS	'cls'
#define		DEL_TRS		'del'
#define 	DEL_TRACK 	'delt'


//extern Mixer*			my_mixer;


typedef struct _x {

		BMediaFile* file;
		BMediaTrack* track;
}	media_file; 


void
transform_to_wav(void* from,void* to,size_t len);

void
melt_to_wav(float** from,void* to,size_t len);

void
melt_to_aiff(float** from,void* to,size_t len);

void
open_export_line(media_file*,entry_ref ref,media_file_format mfi,media_format* mf);

void
close_export_line(media_file*);

JFileManager*
JFileManager::Get()
{
	static	JFileManager*	instance = NULL;
	if (instance == NULL) 
			instance = new JFileManager();
	return instance;
}

JFileManager::JFileManager()
{
	openpanel=NULL;	
	filepanel=NULL;
	open_type=0;
	filtro=new SJFilter();
	errors_log=new BMessage('errs');
	XUtils::GetXRSDirectoryEntry(&sl,"Samples");
	compatible=NULL;
}

void
JFileManager::Init(){
	tm = TrackManager::Get(); //should be removed.
}

Song*
JFileManager::EmptySong()
{
	Song *i= new Song();
	i->Init();
	return i;
}

//void
//JFileManager::CloseSong(Song* s)
//{	
//	delete s;
//}

JFileManager::~JFileManager()
{
	if(filepanel!=NULL) delete filepanel;
	
	if(openpanel!=NULL) delete openpanel;
	
	if(filtro!=NULL) delete filtro;
}
void
JFileManager::SaveReq(entry_ref ref, const char* file_name, Song* s)
{
	BEntry e(&ref);
	BPath  p(&e);
	BEntry* zip;
	
	entry_ref rif;
	
	p.Append(file_name);
	
	status_t er = get_ref_for_path(p.Path(), &rif);
	
	BAlert* idle = XUtils::ShowIdleAlert("Saving...");
	SaveFile(rif, s);
	XUtils::HideIdleAlert(idle);
	
	if(s->getEntry() == NULL)
	{
		s->setEntry(new BEntry(&rif));
	}
	else 
	{
		zip = s->getEntry();
		zip->SetTo(&rif);
		s->setEntry(zip);
	}
	
		
	if ( filepanel != NULL) {
		delete filepanel;
		filepanel = NULL;
	}
}


status_t
JFileManager::Load()
{
	open_type=0;
	entry_ref sl;
	XUtils::GetXRSDirectoryEntry(&sl,"Songs");
	
	if (!openpanel) 
		openpanel = new BFilePanel(B_OPEN_PANEL, &be_app_messenger, &sl,0,false,new BMessage(X_REFS_RECEIVED),NULL,true,true);	
	
	openpanel->SetRefFilter(filtro);
	openpanel->Show();
	return B_OK;
}

#include "PMixer.h"

status_t
JFileManager::LoadFile(entry_ref rif, Song* song)
{
	errors_log->MakeEmpty(); //?
	BMessage errors;
	
	status_t error;
	
	BMessage songInfo;
	
	if (BEntry(&rif).Exists())
	{
		BFile* file = new BFile();   	
	    if ((error = file->SetTo(&rif, B_READ_ONLY) == B_OK) && 
	        (error = file->InitCheck() == B_OK)) 
	    {
				file->Seek(0, SEEK_SET);			
				if ((error = songInfo.Unflatten(file)) != B_OK)
				{
					LogError("Can't decode the file! [%s]", rif.name);
					delete file;
					return error;
				}
				if (songInfo.GetInt64("Version", -1) != FILE_VERSION_INT) {
					errors.AddString("error", "Invalid file version!");
					LogError("Invalid file version! [%s]", rif.name);
					delete file;
					return B_ERROR;
				}
				
		} 
		else  
		{
	    	LogError("Can't read file! [%s]", rif.name);
	    	delete file;
			return error;
		}
		song->setEntry(new BEntry(&rif));
		delete file;
	}

	LogTrace("LoadFile: applying global song info..");
	song->setDescription(songInfo.GetString( "Description", ""));
	song->popupdesc = songInfo.GetBool("DescPopUp", false);
	song->setTempo(songInfo.GetInt64("BeatPerMinute", 120));
	song->getSequence()->all = songInfo.GetBool("PlayAll", false);
	song->setNumberNotes(songInfo.GetInt64("NumNotes", 16));
	
		// Il giro deve essere: dalla song devo salvare il numero di "Measure" e il nome.
	// poi aggiungendo le traccie si dovrebbe creare tutto automaticamente..

	    // Playlist: 
	LogTrace("LoadFile: applying playlist setting..");
	BMessage playlist;
	songInfo.FindMessage("Playlist", &playlist);
	song->getSequence()->LoadSettings(playlist);
	

	LogTrace("LoadFile: applying mixer settings..");	
	BMessage mixer;
	songInfo.FindMessage("Mixer", &mixer);
	PMixer::Get()->LoadSettings(mixer);

	TrackManager*	trackManager = TrackManager::Get();
	
	LogTrace("LoadFile: applying tracks setting..");
	
	BMessage	boostsSettings;
	songInfo.FindMessage("Boosts", &boostsSettings);
	trackManager->LoadBoosterSettings(boostsSettings);
	
	BMessage tracksSettings;
	songInfo.FindMessage("TracksSettings", &tracksSettings);
	int i=0;
	BMessage track;
	while(tracksSettings.FindMessage("TrackSettings", i, &track) == B_OK) {
		int type = track.GetInt16("Type", -1);
		if (type >= 0) {
			Track* cur = trackManager->getTrack(type); //create a track
			song->AddTrack(cur); //create track & sequence linked to measures
			cur->LoadSettings(track);	 //load the settings	
		}

		i++;
	}
	

	LogInfo("\nSong loaded.\n");	
  
	//XUtils::HideIdleAlert(wait);
	
	return B_OK;
	
}

void
JFileManager::Save(Song* s, bool saveas)
{
	entry_ref ref;
	if(s->getEntry() == NULL || saveas==true )
	{
		entry_ref sl;
		XUtils::GetXRSDirectoryEntry(&sl, "Songs");
		if(!filepanel)
			filepanel = new BFilePanel(B_SAVE_PANEL,NULL,&sl,0,false,NULL,NULL,true);
		filepanel->SetRefFilter(filtro);
		filepanel->Show();
			
 	}
	else
	{
		s->getEntry()->GetRef(&ref);
		
		BAlert* idle = XUtils::ShowIdleAlert("Saving...");
		SaveFile(ref, s);
		XUtils::HideIdleAlert(idle);
	}
	
}

	

int64
JFileManager::ReadFixed(int64 *data)
{
	int64 code;
	if(file->Read(&code,sizeof(int64))==0)
		return SONG_STOP;
	file->Read(data,sizeof(int64));
	return code;	
}
int64
JFileManager::ReadVar(void *data)
{
	int64 code;
	int64 size;
	code=ReadFixed(&size);
	
	file->Read(data,size);
	
	return code;	
}
void JFileManager::ExportWave(BMessage *info)
{
	if(!filepanel)
	filepanel=new BFilePanel(B_SAVE_PANEL,NULL,NULL,0,false,NULL,NULL,true);
	filepanel->Show();
	filepanel->SetMessage(info);
}

status_t
open_export_line(media_file *dest,entry_ref ref,media_file_format* mfi, media_format* mf)
{
	status_t err;
	
	dest->file=new BMediaFile(&ref,mfi);
	err=dest->file->InitCheck();
	
	if(err!=B_OK) return err;
			
	dest->track=dest->file->CreateTrack(mf);
	BString copy;
	copy << "Created with" << VERSION_NUMBER << "by Anzani Andrea";
	dest->file->AddCopyright(copy.String());
	dest->file->CommitHeader();
	
	return B_OK;
}
void
close_export_line(media_file *dest)
{
	 dest->track->Flush();
	 dest->file->ReleaseTrack(dest->track);
	 dest->file->CloseFile();
}

status_t
JFileManager::RenderAsWave(BMessage *message)
{
	return B_ERROR;
	/*
	 // decoding BMessage
	 entry_ref ref;
	 entry_ref rif;
	 int form;
	 bool all;
	 int pattern;
	 int32	lines;
	 
	 if(message->FindRef("directory",&rif)!=B_OK) return B_ERROR;
	 
	 
	BEntry e(&rif);
	BPath p(&e);

	p.Append(message->FindString("name"));
	get_ref_for_path(p.Path(),&ref);
	printf("works %s\n",p.Path());
				
	form=message->FindInt16("format");
	all=message->FindBool("playmode");
	pattern=message->FindInt16("position");
	
	lines=message->FindInt16("lines");
	
	if(lines==1) lines=my_mixer->CountLines();
	else lines++;
	//
    if(filepanel!=NULL) 
	{
		delete filepanel;
		filepanel=NULL;
	}
     printf("render : %d %d %d\n",form,all,pattern);
     
     status_t err;
     entry_ref refrec;  
     media_file_format mfi;
     media_format mf,outfm;
     int32 cookie=0;
     
     char *filetype;
     void *buffer;
 	 void *newbuffer;
 	 int32	buffer_size;
 	 
 	 // all track
 	
 	 
 	 int numTrack;
 	 numTrack=lines;
 //	 int allTrack;
// 	 allTrack=true;
 	 media_file*	inuse;
 	 inuse=new media_file[lines];
 	 
 	 for(int i=0;i<lines;i++) inuse[i].file=NULL;
 	 
 	  	 
	 BAlert * al;
	 BButton *theb;
	 al=new BAlert("XRS",T_DIALOG_EXPORT_AS_WAVE,T_GEN_DONE);
	 
	 XUtils::SetIdleAlert(al);
	 	 
	 theb=(al->ButtonAt(0));
	 theb->Hide();
	 al->Go(NULL);
	
     //BMediaFile *dest;
     //BMediaTrack	*rectrack;

	 media_raw_audio_format *fmt;
	 fmt=&mf.u.raw_audio;
	 mf.type=B_MEDIA_RAW_AUDIO;
	 

	 switch(form){
	
		case 0: //SHORT 	
	 	
	 		filetype="wav";
	 		fmt->format=media_raw_audio_format::B_AUDIO_SHORT;
			fmt->channel_count=2;
			fmt->frame_rate=44100;
			fmt->byte_order=B_MEDIA_LITTLE_ENDIAN;
			fmt->buffer_size=FRAMES_NUM*4;
			
			newbuffer=malloc(FRAMES_NUM*4);
	
	  	break;
	  	
	 	case 1:
	 		filetype="AIFF";
	 		fmt->format=media_raw_audio_format::B_AUDIO_FLOAT;
			fmt->channel_count=2;
			fmt->frame_rate=44100;
			fmt->byte_order=B_MEDIA_LITTLE_ENDIAN;
			fmt->buffer_size=FRAMES_NUM*8;
			
			newbuffer=NULL;
			
	 	default:
	 		//goto cocco;
	 	break;
	}
	
	buffer_size=fmt->buffer_size;
	
	buffer=malloc(FRAMES_NUM*8); // il buffer da passare a ProcessBuff.. SEMPRE da 8 byte..
	
	   while((err=get_next_file_format(&cookie,&mfi))==B_OK){
		
		printf("format %s\n",mfi.short_name);
	 	if(strcmp(mfi.short_name,filetype)==0)
	 	break;
	 	
	  }
	
	succo->BeginExport();	
	
	//if(err!=B_OK) goto cocco;

	
	//BString newname;
	//char* oldname;
	//oldname=new char[strlen(ref.name)];
	//strncpy(oldname,ref.name,strlen(ref.name));
	BString basename;
	basename.SetTo(ref.name);
	BString	newname(basename);
	//if(err!=B_OK) goto cocco;
			
	// version 1.4;
	
	while(!succo->turn){
		
		
		succo->ProcessBuffer((void *)succo,buffer,FRAMES_NUM*8);
		
		for(int i=0;i<numTrack;i++)
		{
		
		 if( my_mixer->IsEnableLine(i)) 
		 {		
		 	if(inuse[i].file==NULL){	
		 		
		 		newname.SetTo(basename);
		 		if(i!=0) {
		 				newname.Append(" (");
		 				newname.Append(my_mixer->getLineName(i));
		 				newname.Append(")");
		 		}
		 		ref.set_name(newname.String());	
		 		open_export_line(&inuse[i],ref,&mfi,&mf);
		 }
		 
		 if(form==0) //SHORT 
			 {
			 	melt_to_wav(my_mixer->getBufferLine(i),newbuffer,FRAMES_NUM);
			   //	transform_to_wav(buffer,newbuffer,FRAMES_NUM);
				inuse[i].track->WriteChunk((char*)newbuffer,FRAMES_NUM*4);
			 }
	  	 else
	  	
	 	 if(form==1){
	 	 		melt_to_aiff(my_mixer->getBufferLine(i),buffer,FRAMES_NUM);	
				inuse[i].track->WriteChunk((char*)buffer,FRAMES_NUM*8);	
		
		}
		} 		
		}	
	 	
 }
 
 // qui manca ancora una nota da renderizzare
 //succo->last_buf_good
 size_t left;
 size_t chuck;
 
 left=succo->note_size/4-(FRAMES_NUM-succo->last_buf_good);
 
 while(left)
 {
 	if(left>=FRAMES_NUM) chuck=FRAMES_NUM;
 	else chuck=left;
 	
 		
 		succo->ProcessBuffer((void *)succo,buffer,chuck*8);
 		for(int i=0;i<numTrack;i++)
		{
		 if(inuse[i].file!=NULL)
		 {	
			if(form==0) //SHORT 
			{
			    melt_to_wav(my_mixer->getBufferLine(i),newbuffer,chuck);
			   	//transform_to_wav(buffer,newbuffer,chuck);
				inuse[i].track->WriteChunk((char*)newbuffer,chuck*4);
			}
	  		else
	  	
	 		if(form==1){
	 	 		melt_to_aiff(my_mixer->getBufferLine(i),buffer,chuck);	
				inuse[i].track->WriteChunk((char*)buffer,chuck*8);	
		 	}
		 }
		}
		left -=chuck;
 	
  	} 
		
		

		// close files:
		for(int i=0;i<numTrack;i++)
		{
		
		 if(inuse[i].file!=NULL)			
		 		close_export_line(&inuse[i]);
			
		}
		//close_export_line(&inuse[0]);
	    
	    free(buffer);
	    free(newbuffer);
	    
	    printf("Render finished!\n");
	  	
	  	//identify
	  	BMimeType result;
		BMimeType::GuessMimeType(&ref,&result);
		BFile newFile(&ref,B_READ_WRITE);
		BNodeInfo ninfo(&newFile); 
		ninfo.SetType(result.Type());
	    
	   if(al->Lock())
	   {
	   	theb->Show();	 
	  	XUtils::StopIdleAlert(al);
	  	al->Unlock();
	   }
	
	  succo->EndExport();
		
	  return B_OK;
	  

cocco:
	
	ErrorMsg("An error occorred while exporting! :",strerror(err));
	if(al->Lock()) al->Quit();
	succo->EndExport();
	return err;
	*/
}

void
melt_to_wav(float** from,void* to,size_t frames)
{
	int	r;
	for(size_t i=0;i<frames;i++)
	{
		
		r=(int)(from[0][i]*32766);
		memcpy((char*)to+i*4,(void*)&r,2);
		//rights
		r=(int)(from[1][i]*32766);
		memcpy((char*)to+i*4+2,(void*)&r,2);
	}
}

void
melt_to_aiff(float** from,void* to,size_t frames)
{
	float r;
	for(size_t i=0;i<frames;i++)
	{
		
		r=from[0][i];
		memcpy((char*)to+i*8,(void*)&r,4);
		//rights
		r=from[1][i];
		memcpy((char*)to+i*8+4,(void*)&r,4);
	}
}

void
transform_to_wav(void* from,void* to,size_t frames)
{
	float f;
	int	r;
	for(size_t i=0;i<frames;i++)
	{
		// left
		memcpy((void*)&f,(char*)from+i*8,4);
		r=(int)(f*32766);
		memcpy((char*)to+i*4,(void*)&r,2);
		//rights
		memcpy((void*)&f,(char*)from+i*8+4,4);
		r=(int)(f*32766);
		memcpy((char*)to+i*4+2,(void*)&r,2);
	}
	
}

void
JFileManager::AddMime(BFile*	 file)
{
	BNodeInfo	info;
	info.SetTo(file);
	info.SetType("audio/XRS-File");
}
status_t
JFileManager::AskForClose(Song* s)
{	
	BAlert *sam;
	int32 sam_say;

	//if(!s->isModified()) return B_OK;
	
			
	sam=new BAlert("XRS",T_DIALOG_CLOSE_SONG,T_GEN_CLOSE,T_GEN_NO);

	sam_say=sam->Go();
	
	if(sam_say==0) return B_OK;
	else return B_ERROR;
		
}
void
JFileManager::ErrorMsg(const char *msg,const char *txt)
{
	char pippo[strlen(msg)+strlen(txt)+2];
	
	strcpy(pippo,msg);
	strcat(pippo,"\n");
	strcat(pippo,txt); 
	(new BAlert("XRS", pippo, T_GEN_OK,NULL,NULL,B_WIDTH_AS_USUAL,B_WARNING_ALERT))->Go();	
}
void
JFileManager::AnalizeError(BMessage* msg)
{
	if(errors_log==NULL) return;
	BString error_str;
	int i=0;
	while(msg->FindString("error",i,&error_str)==B_OK)
	{
		//printf("ERROR: %s",error_str.String());
		errors_log->AddString("error",error_str);
		i++;
	}
}



//
// NEW VERSION All BMessage Based!
#include "PMixer.h"
status_t
JFileManager::SaveFile(entry_ref rif, Song* song)
{
	status_t error;
		
	BFile* file =new BFile();
   	
    if ((error = file->SetTo(&rif,B_CREATE_FILE|B_READ_WRITE) != B_OK)) {
		LogError("Can't create file! (SetTo)");
		return error;
	}
   	
    if (( error = file->InitCheck() != B_OK)) {
    	LogError("Can't create file! (InitCheck)");
		return error;
	}

	//TODO MOVE TO THE SONG CLASS??!?
	BMessage songInfo;
	songInfo.AddInt64  ( "Version" 		, FILE_VERSION_INT);
	songInfo.AddString ( "Description"	, song->getDescription());
	songInfo.AddBool   ( "DescPopUp"    , song->popupdesc);
	songInfo.AddInt64  ( "BeatPerMinute", song->getTempo());
	songInfo.AddBool   ( "PlayAll"      , song->getSequence()->all);
 	songInfo.AddInt64  ( "NumNotes"		, song->getNumberNotes());
 	// Playlist: 
	BMessage playlist;
	song->getSequence()->SaveSettings(playlist);
    songInfo.AddMessage("Playlist", &playlist);

 	

	//saving Mixer information.
	BMessage mixer;
	PMixer::Get()->SaveSettings(mixer);
	songInfo.AddMessage("Mixer", &mixer);


	//Saving tracks info
	TrackManager*	trackManager = TrackManager::Get();
	
	BMessage	boostsSettings;
	trackManager->SaveBoosterSettings(boostsSettings);
	songInfo.AddMessage("Boosts", &boostsSettings);
	

    
	BMessage tracksSettings;
	for(int j=0;j<song->getNumberTrack();j++)
	{
		// Track Saving
		BMessage track;
		song->getTrackAt(j)->SaveSettings(track);
		tracksSettings.AddMessage("TrackSettings", &track);
	}

	songInfo.AddMessage("TracksSettings", &tracksSettings);    
    

  	
	songInfo.Flatten(file);
    AddMime(file);    	
    delete file;
	LogInfo("\nFile Saved!\n");
	return B_OK;	
}
