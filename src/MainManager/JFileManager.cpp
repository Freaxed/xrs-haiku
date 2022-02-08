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

void
JFileManager::CloseSong(Song* s)
{	
	delete s;
}

JFileManager::~JFileManager()
{
	if(filepanel!=NULL) delete filepanel;
	
	if(openpanel!=NULL) delete openpanel;
	
	if(filtro!=NULL) delete filtro;
}
void
JFileManager::SaveReq(entry_ref ref, const char* file_name,Song* s)
{
	BEntry e(&ref);
	BPath p(&e);
	
	BEntry* zip;
	
	entry_ref rif;
	//bool p_r;
	
	status_t er;
	
	
	p.Append(file_name);
	
	er=get_ref_for_path(p.Path(),&rif);
	
	printf("Do you want to load : %s  status : %s \n",p.Path(),strerror(er));
	
	
	
	
			
			SaveFile(rif,s);
			if(s->getEntry()==NULL) s->setEntry(new BEntry(&rif));
			else {
				zip=s->getEntry();
				zip->SetTo(&rif);
				s->setEntry(zip);
			   }
			//s->setModified(false);
		
		
	if(filepanel!=NULL) 
	{
		delete filepanel;
		filepanel=NULL;
	}
}

// status_t
// JFileManager::SaveFile(entry_ref rif,Song* song,bool zip)
// {
	
// 	status_t error;
// 	int i,j,k;
	
// 	file=new BFile();
				  
//     char	title[10]="ClapLost ";
    	
//     error=file->SetTo(&rif,B_CREATE_FILE|B_READ_WRITE);
//     if(error) return error;
    	
//     error=file->InitCheck();
//     if(error) return error;

    	
    	
//     	WriteFixed(FILE_VERSION,FILE_VERSION_INT);
//        	WriteFixed(SONG_START,0);
   
//     	WriteVar(SONG_TITLE,title,10);
//     	WriteVar(SONG_DESC,(void*)song->getDescription(),song->getSizeDescription());
    
//     	if(song->popupdesc)
//     		WriteFixed(SONG_POPUP_DESC,1);
//     	else
//     		WriteFixed(SONG_POPUP_DESC,0);
	    	
 	   	
//     	Track*		cur;
//     	Pattern* 	pat;
//     	BMessage*	m_data;
//     	int64	ap;
    	
//     	WriteFixed(SONG_TIME,song->getTempo());
//     	if(song->getSequence()->all)
//     		WriteFixed(SONG_PLAY_ALL,song->getTempo());
    	
    	   	
//     	//Mixer (OutputLines & VSTi)
//     	song->mixer_settings->MakeEmpty();
//     	MixerWindow::Get()->SaveSettings(song->mixer_settings);
//     	WriteFixed(MIXER_SETTINGS,song->mixer_settings->FlattenedSize());
// 		song->mixer_settings->Flatten(file);   	
    	
//     	int curType;    	
    	
//     	int32 n=(int32)song->getNumberNotes();
//     	WriteFixed(SONG_NUM_NOTES,n);
    	    	
//     	for(int i=0;i<MAX_PLUG;i++)
// 		{
// 			if(tm->isBoosterValid(i))
// 			{
// 				 //WriteFixed(SONG_TRACKBOOST,i);
// 				m_data=new BMessage();
// 				if(zip) m_data->AddBool("zipped",true);
				
// 				tm->SaveBoosterSettings(i,m_data);
//     			WriteFixed(BOOSTER_SETTINGS,m_data->FlattenedSize());
// 				m_data->Flatten(file);   	
// 				if(zip)
// 					CollectData(m_data,rif);
				
// 			}
// 		}	
    	
//     	for(j=0;j<song->getNumberTrack();j++)
//     	{
    	
//     		// Track Saving
//     		// Common track properties (vol/pan/name/pattern)
//     		// & Specific Type-depended info (with a BMessage)
    		
//     		cur=song->getTrackAt(j);
//     		curType=cur->getModel();
    		
    		   		    		
//     		WriteFixed(TRACK_START,curType);
    		 
    		 
//     		WriteFixed(TRACK_VOLUME,(int64)(cur->Gain() * 100));
//     		WriteFixed(TRACK_PANNER,(int64)(cur->Pan() * 100));
    		
    		
//     		if(cur->isNameLocked()) 
//     		{
//     			// first you set the new name.   			
// 				WriteVar(TRACK_NAME,(void*)cur->getName(),strlen(cur->getName()));
				
// 				// second you lock that name
// 				ap=1;
//     			WriteFixed(TRACK_NAME_SET,ap);
// 			}
			
// 			ap = cur->isOn() ? 1 : 0 ;
    		
//     		WriteFixed(TRACK_MUTE,ap);
//     		WriteFixed(TRACK_OUTPUT_LINE,cur->getRouteLine());	
		 	
// 		 	// salvataggio MIDI-IN??
// 		 	WriteFixed(TRACK_MIDI_IN_CH,cur->GetMidiInCh());
		 	
// 		 	ap = cur->IsMidiInEnable() ? 1 : 0 ;
		 	
// 		 	WriteFixed(TRACK_MIDI_IN_EN,ap);
		 	
		 	
//      		m_data=new BMessage();
//     		tm->SaveTrackSettings(cur,m_data);
//     		WriteFixed(TRACK_SETTINGS,m_data->FlattenedSize());
// 			m_data->Flatten(file); 	
     		
//     		 for(i=0;i<song->getNumberMeasure();i++)
//     		 {
    	
//     			 WriteFixed(PATTERN_START,0);
    			
//     			 pat=cur->getPatternAt(i); 
    			
//     			 for(k=0;k<pat->getNumberNotes();k++)
//     			 {
//     			    	WriteFixed(NOTE_START,0);
    			    	
//     			    		if(pat->getNoteAt(k)->getValue())
//     			    			WriteFixed(NOTE_ON,1);
//     			    		else
//     			    			WriteFixed(NOTE_ON,0);
    			    	
//     			    		WriteFixed(NOTE_VOLUME,(int64)(pat->getNoteAt(k)->Gain()*100));
//     			    		WriteFixed(NOTE_NEWNOTE,pat->getNoteAt(k)->getNote());
    			    	
//     			    	WriteFixed(NOTE_STOP,0);		
//     			}		
    			
//     			 WriteFixed(PATTERN_STOP,0);
    		  			
//     		 }
         		
//       	WriteFixed(TRACK_STOP,curType);
// 	} 
    
    
//     // Playlist:
    
//     	WriteFixed(PL_START,1);
    	
//     	WriteFixed(PL_LOOP_ENABLE,(int)song->getSequence()->loop_enable);
//     	WriteFixed(PL_LOOP_POINT1,song->getSequence()->loop_points[0]);
//     	WriteFixed(PL_LOOP_POINT2,song->getSequence()->loop_points[1]);
    	
//     	for(int p=0;p<song->getSequence()->getMaxSeq()+1;p++)
//     	{
    		
//     			WriteFixed(PL_POS_START,p);
//     			/*Inserire altro ciclo loop*/
//     			for(int jx=0;jx<song->getSequence()->getMaxPat();jx++)
//     			{
//     				int y=song->getSequence()->ItemAt(p,jx);
//     				if(y>=0)	WriteFixed(PL_POS_PAT,jx);
    				
//     			}
//     			WriteFixed(PL_POS_STOP,0);
//     	}
    	
//     	WriteFixed(PL_STOP,1);
    		
//     	WriteFixed(SONG_STOP,0);
    	
//     	AddMime(file);
    	
//     	delete file;
	
// 	return B_OK;
	
// 	printf("File Saved!\n");
	
// }

status_t
JFileManager::Load()
{
	open_type=0;
	entry_ref sl;
	XUtils::GetXRSDirectoryEntry(&sl,"Songs");
	
	if(!openpanel) 
		openpanel=new BFilePanel(B_OPEN_PANEL,&be_app_messenger,&sl,0,false,new BMessage(X_REFS_RECEIVED),NULL,true,true);	
	
	openpanel->SetRefFilter(filtro);
	openpanel->Show();
	return B_OK;
}


//void
//JFileManager::setRef(entry_ref ref)
//{
//	BPath p(&ref);
//	if(p.GetParent(&p)==B_OK)	get_ref_for_path(p.Path(),&sl);	
//	printf("current : %s\n",sl.name);
//}

status_t
JFileManager::LoadFile(entry_ref rif,Song* song)
{
	
	status_t error;
	char	title[100];
	//char	Filename[100];
	char	base[SAMPLE_NAME_L];
	//char	path[B_PATH_NAME_LENGTH];
	entry_ref ref;
	bool	have_seq=false;
	
	errors_log->MakeEmpty();
	
	
	file=new BFile();
    	
    error=file->SetTo(&rif,B_READ_ONLY);
    	if(error) return error;
    error=file->InitCheck();
    	if(error) return error;

	file->Seek(0,SEEK_SET);

			
	int64 rt,va=0;
	int64 fileversion;

	if(FILE_VERSION!=ReadFixed(&rt)) return B_ERROR;
	printf("File version : %lld\n",rt);
	fileversion=rt;
	if(fileversion<2) return B_ERROR;
	if(SONG_START!=ReadFixed(&rt)) return B_ERROR;
	

	BAlert* wait = XUtils::ShowIdleAlert(T_DIALOG_LOADING_FILE);

	 
	int track_c=0;
	Track*	track_o;
	int pat_c=0;
	int cur_pos=0;
	int note_c=0;

	int track_id=0;
	BMessage	*d_msg;
	BString	error_str;
	
	while(va!=SONG_STOP)
	{
		va=ReadFixed(&rt);
		switch(va){
		
			case 	SONG_NUM_NOTES:
				song->setNumberNotes(rt);
			break;
			case	SONG_POPUP_DESC:
				if(rt==0) song->popupdesc=false; 
					else 	song->popupdesc=true;
			break;
			case SONG_TITLE:
				file->Read(title,rt);
				title[rt]='\0';
			break;
			case SONG_DESC:
				{
				char ptr[rt+1];
				file->Read(ptr,rt);
				ptr[rt]='\0';
				song->setDescription((const char*)ptr);
				}
			break;
			
			/* very old file type?? 
			case BANK_NAME:
				//printf("Found a bank! %d\n",rt);
				//tm->Load(NULL,0,file,va,rt)	;			
			break;
			case EXT_LIST_START:
				//printf("Found an external list of samples! \n");
				//tm->Load(NULL,0,file,va,rt)	;
			break;*/
			
			case PL_START:
				have_seq=true;
				while(va!=PL_STOP)
				{
					va=ReadFixed(&rt);
					switch(va){
						case PL_LOOP_ENABLE:
						song->getSequence()->loop_enable=(bool)rt;
						break;
						case PL_LOOP_POINT1:
						song->getSequence()->loop_points[0]=rt;
						break;
						case PL_LOOP_POINT2:
						song->getSequence()->loop_points[1]=rt;
						break;
						case PL_POS_START:
							
							cur_pos=rt;
							while(va!=PL_POS_STOP)
							{
								va=ReadFixed(&rt);
								switch(va){
									case PL_POS_PAT:
									song->getSequence()->setItemAt(cur_pos,rt,true);
									break;
								default:
									if(va>=2000) file->Seek(rt,SEEK_CUR);
								break;
								}
							}
						break;
						default:
							if(va>=2000) file->Seek(rt,SEEK_CUR);
						break;
					}
				}
			break;
			case SONG_VST_START:
			{
				// only for compatibily purpose with 1.2
				
				BMessage	temp_line;  // proprietà linea corrente
				BMessage	vst;	    // preset vst corrente
				
				d_msg=song->mixer_settings;
				d_msg->MakeEmpty();
				d_msg->AddInt16("version",0);
										
				int  pos=0;
				
				//debugger("mixer");
				temp_line.MakeEmpty();
				
				while(va!=SONG_VST_STOP){
									
					va=ReadFixed(&rt);
					
					switch(va){
						
						case SONG_VST_NAME:
							file->Read(title,rt);
							title[rt]='\0';
							
							vst.AddInt16("position",pos);
							vst.AddString("leaf",BString(title));
							pos++;
						break;
					
						case SONG_VST_DATA:
							{
							 BMessage newdata;
							 newdata.Unflatten(file);
							 printf("Data\n");
							 vst.AddBool("active",newdata.FindBool("active"));
							 vst.AddMessage("preset",&newdata);
							 newdata.MakeEmpty();
							 vst.AddMessage("window",&newdata);
							 						 
							 /* aggiunta di 2 bmessage */
							}
						break;
						
						case SONG_VST_LINE:
							/* chiudere la precedente??*/
							if(!temp_line.IsEmpty())
							{
								d_msg->AddMessage("line_settings",&temp_line);
								d_msg->AddMessage("vst_settings",&vst);
								vst.MakeEmpty();	
								pos=0;
							}
							
							temp_line.MakeEmpty();
							temp_line.AddInt16("id",rt+1);
							temp_line.AddFloat("vol",0.8);
							temp_line.AddFloat("pan",0.0);
						
						break;
						
						case SONG_VST_GLOBAL:
							/* chiudere la precedente??*/
							if(!temp_line.IsEmpty())
							{
								d_msg->AddMessage("line_settings",&temp_line);
								d_msg->AddMessage("vst_settings",&vst);
								vst.MakeEmpty();
								pos=0;
							}
							
							temp_line.MakeEmpty();
							temp_line.AddInt16("id",0);
							temp_line.AddFloat("vol",0.8);
							temp_line.AddFloat("pan",0.0);
							
						break;
						
						default:
							if(va>=2000) 
								file->Seek(rt,SEEK_CUR);
						break;
					}
				}
							
			if(!temp_line.IsEmpty())
			{
				d_msg->AddMessage("line_settings",&temp_line);
				d_msg->AddMessage("vst_settings",&vst);
				vst.MakeEmpty();
			}
			
			}	
				
			break;
			case MIXER_SETTINGS:
				
				song->mixer_settings->MakeEmpty();
				song->mixer_settings->Unflatten(file);
		   			
			break;	
			case SONG_TIME:
				song->setTempo((int)rt);
			break;
			case SONG_PLAY_ALL:
				song->getSequence()->all=true;
				break;
			
			case SONG_TRACKBOOST: //DEPRECATED!!
				// tm->Load(NULL,rt,file,va,rt);
				// here we shold enable a false-function in order to
				// load version 2 files
				// but only version 2!
				
				//if(fileversion!=2)
				
				printf("SONG_TRACKBOOST %lld\n",rt);
				if(rt==0)
				{
				
				d_msg=new BMessage();
				d_msg->Unflatten(file);
				
				BMessage *newmess=new BMessage();
				newmess->AddMessage("samples_list",d_msg);
				newmess->AddRef("Song_ref",&rif);
				newmess->PrintToStream();
				tm->LoadBoosterSettings(newmess);
				AnalizeError(newmess);
				delete d_msg;
				delete newmess;
				}
				break;
			case BOOSTER_SETTINGS:
				d_msg=new BMessage();
				d_msg->Unflatten(file);
				d_msg->AddRef("Song_ref",&rif);
				tm->LoadBoosterSettings(d_msg);
				AnalizeError(d_msg);
				delete d_msg;
			break;
			case TRACK_START:
				
				track_o=tm->getTrack(rt);
				track_id=rt;
				track_c=song->MeltTrack(track_o);
				
				while(va!=TRACK_STOP)
				{
					va=ReadFixed(&rt);
					switch(va){

					case TRACK_NAME:
						file->Read(base,rt);
						base[rt]='\0';
						printf("Loading Track name %s\n",base);
						song->getTrackAt(track_c)->setName(base);
					break;
					case TRACK_NAME_SET:
						if(rt)
						song->getTrackAt(track_c)->LockName(true);
						else
						song->getTrackAt(track_c)->LockName(false);
					
					break;
					case TRACK_VOLUME:
						song->getTrackAt(track_c)->SetGain((float)rt/(float)100);
					break;
					case TRACK_PANNER:
						song->getTrackAt(track_c)->SetPan((float)rt/(float)100);
					break;
					case TRACK_MUTE:
						if(rt==0)
						song->getTrackAt(track_c)->setOn(false);
							else
						song->getTrackAt(track_c)->setOn(true);
					break;
					case TRACK_FX1_LINE: //for 1.2
						song->getTrackAt(track_c)->setRouteLine(rt+1);
					break;
					case TRACK_FX1:		//for 1.2
						if(rt==0)
							song->getTrackAt(track_c)->setRouteLine(0);
					break;
					case TRACK_OUTPUT_LINE:
						song->getTrackAt(track_c)->setRouteLine(rt);
					break;
					case TRACK_MIDI_IN_EN:
						if(rt==0)
							song->getTrackAt(track_c)->SetMidiInEnable(false);
						else
							song->getTrackAt(track_c)->SetMidiInEnable(true);
					break;
					case TRACK_MIDI_IN_CH:
						song->getTrackAt(track_c)->SetMidiInCh(rt);
					break;
					//
					//deprecated just for compatibility with 1.2
					
					//sampler
					case TRACK_RESAMPLE_EN:
					case TRACK_AMPBOOST:
					case TRACK_RESAMPLE:
					case TRACK_REVERSED:
					case TRACK_SAMPLE:
					
					//tn303
					case TRACK_SPECIFIC_DATA_START:
				
					Comatible12(song->getTrackAt(track_c),va,rt);
					break;
					///end deprecated
					case TRACK_SETTINGS:
						d_msg=new BMessage();
						d_msg->Unflatten(file);
						tm->LoadTrackSettings(song->getTrackAt(track_c),d_msg);
						AnalizeError(d_msg);
						delete d_msg;
					break;
					case PATTERN_START:
				
						song->AddMeasure();
						
						while(va!=PATTERN_STOP)
						{
							va=ReadFixed(&rt);
							switch(va){
							
							case NOTE_START:
								while(va!=NOTE_STOP)
								{
									va=ReadFixed(&rt);
									switch(va){
									
										case NOTE_ON:
										if(rt==0)
										{
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setValue(false);
										// printf("-");
										}
										else
										{
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setValue(true);
										//printf("*");
										}
										break;
										case NOTE_VOLUME:
										
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->SetGain((float)rt/100.);

										break;
										case NOTE_OCT:
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setOct(rt);
										break;
										
										case NOTE_NOTE:
										// song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setNote(rt);
										 //old message :
										 // 1) pulire i primi due bit di oct
										 // 2) ricalcolare in base MIDI
										 {
										 int oc=song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->getOct();
										 
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setOct(oc&8);
										 
										 oc=oc&7;
										 
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setNote(rt+oc*12+60);
										
										}
										break;
										case NOTE_NEWNOTE:
										 song->getTrackAt(track_c)->getPatternAt(pat_c)->getNoteAt(note_c)->setNote(rt);
										break;
										default:
										//printf("3 Unknown message! %lld\n",va);
										if(va>=2000) file->Seek(rt,SEEK_CUR);
										break;																			
									}
								}
								note_c++;
							break;
							
							default:
							if(va>=2000) file->Seek(rt,SEEK_CUR);
							
							break;
							}
						}
						note_c=0;
						pat_c++;
					break;
					default:
					//printf("1 Unknown message! %lld\n",va);
					//printf("DEBUG: track id %d\n",track_id);
					//bool ing=tm->Load(track_o,track_id,file,va,rt);
					if(va>=2000) file->Seek(rt,SEEK_CUR);
					
					break;
					}
				}
				//track_c++;
				song->setNumberMeasure(pat_c);
				pat_c=0;
			break;
			
			default:
			if(va>=2000) file->Seek(rt,SEEK_CUR);
			
			break;
	}
	
}		

	if(!have_seq)	{
		//printf("Song without a sequnece!\n");
		song->getSequence()->Reset();
		song->getSequence()->all=true;
		}
	//printf("Song is Ok? %s\n",song->getBKM()->the_ref.name);
	song->setEntry(new BEntry(&rif));
	//song->setModified(false);
	printf("Song loaded.\n");	
	delete file;
	  
	XUtils::HideIdleAlert(wait);
	
	return B_OK;
	
}

void
JFileManager::Save(Song* s,bool saveas)
{

	entry_ref ref;

	if(s->getEntry()==NULL || saveas==true )
		
		{

			entry_ref sl;
			XUtils::GetXRSDirectoryEntry(&sl,"Songs");
			if(!filepanel)
				filepanel=new BFilePanel(B_SAVE_PANEL,NULL,&sl,0,false,NULL,NULL,true);
			filepanel->SetRefFilter(filtro);
			filepanel->Show();
			
	 	}
	 else
	 	{
	 		//printf("Second case!\n");
	 		s->getEntry()->GetRef(&ref);
	 		SaveFile(ref,s);
	 		//s->setModified(false);
	 	}
	
}

	
void
JFileManager::WriteFixed(int64 code ,int64 data)
{
	file->Write(&code,sizeof(int64));
	file->Write(&data,sizeof(int64));
	return;
}

void
JFileManager::WriteVar(int64 code ,void* data,int64 size)
{
	WriteFixed(code,size);
	file->Write(data,size);		
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


void
JFileManager::Comatible12(Track* tr,int32 va,int32 rt)
{ //->
	int tracktype=tr->getModel();
	if(tracktype==0)
	{
		switch(va)
		{	// we are in version 2!
			case TRACK_SAMPLE:
			{
				compatible= new BMessage();
				char ptr[rt+1];
				file->Read(ptr,rt);
				ptr[rt]='\0';
				compatible->AddString("sample_name",BString(ptr));
			}
			break;
			case TRACK_REVERSED:
				if(!compatible) compatible=new BMessage();
				compatible->AddBool("reverse",(bool)rt);
			break;
			case TRACK_RESAMPLE_EN:
				compatible->AddBool("resample_enable",(bool)rt);
				break;
			case TRACK_RESAMPLE:
				compatible->AddInt16("resample",(int16)rt);
				break; 	
			case TRACK_AMPBOOST:
				compatible->AddInt16("ampboost",(int16)rt);
				if(rt>1)
				compatible->AddBool("boost_enable",true);
				// this is the last sample1.2 message:
				tm->LoadTrackSettings(tr,compatible);
				AnalizeError(compatible);
				delete compatible;
				compatible=NULL;
				break;
			default:break;
				
		}
	} else
	if(tracktype==1) //Tn303 version 1.2
	{
		switch(va){
		
			case TRACK_SPECIFIC_DATA_START:
			{
				BMessage msg;
				msg.Unflatten(file);
				tm->LoadTrackSettings(tr,&msg);
				AnalizeError(&msg);
			}
			break;
		}
	} else
	if(tracktype==10) //VIW version 1.2
	{
		switch(va){
		
			case TRACK_SPECIFIC_DATA_START:
			{
				//debugger("vst! settings??! GetConfig!");
				BMessage msg;
				msg.Unflatten(file);
				msg.PrintToStream();
				
				BMessage d_msg;
				d_msg.AddString("plugin_name",BString("mdaJX10"));
				d_msg.AddMessage("window_settings",&msg);
				d_msg.AddMessage("instrument_preset_old",&msg);
				tm->LoadTrackSettings(tr,&d_msg);
				AnalizeError(&msg);
			}
			break;
		}
	}
}

//
// NEW VERSION All BMessage Based!
#include "PMixer.h"
status_t
JFileManager::SaveFile(entry_ref rif,Song* song, bool zip)
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
    songInfo.AddString ( "Title"   		, "");
	songInfo.AddString ( "Description"	, song->getDescription());
	songInfo.AddBool   ( "DescPopUp"    , song->popupdesc);
	songInfo.AddInt64  ( "BeatPerMinute", song->getTempo());
	songInfo.AddBool   ( "PlayAll"      , song->getSequence()->all);
 	songInfo.AddInt64  ( "NumNotes"		, song->getNumberNotes());

	//saving Mixer information.
	BMessage mixer;
	for (uint8 i=0; i<PMixer::Get()->CountBusses(); i++) {
		BMessage mixerLine;
		PBus* bus = PMixer::Get()->BusAt(i);
		mixerLine.AddFloat("Gain", bus->Gain());
		mixerLine.AddFloat("Pan",  bus->Pan());
		BMessage fxs;
		bus->Effector()->SaveSettings(&fxs);
		mixerLine.AddMessage("Effects", &fxs);

		mixer.AddMessage("Line", &mixerLine);
	}
	songInfo.AddMessage("Mixer", &mixer);


	//Saving tracks info
	TrackManager*	trackManager = TrackManager::Get();
	
	BMessage	boostsSettings;
	for(int16 i=0;i<MAX_PLUG;i++)
	{
		if(trackManager->isBoosterValid(i))
		{
			BMessage boost;
			boost.AddInt16("id", i);
			
			trackManager->SaveBoosterSettings(i, &boost);
			boostsSettings.AddMessage("BoostSettings", &boost);		
		}
	}

	songInfo.AddMessage("Boosts", &boostsSettings);
    
	BMessage tracksSettings;
	for(int j=0;j<song->getNumberTrack();j++)
	{
	
		// Track Saving
		BMessage track;

		Track* cur = song->getTrackAt(j);
		
		track.AddInt16("Type", cur->getModel());				
		track.AddFloat("Gain", cur->Gain());
		track.AddFloat("Pan",  cur->Pan());					
		
		if(cur->isNameLocked()) 
		{
			track.AddString("Name",		  cur->getName());		
			track.AddBool  ("NameLocked", true);
		}
		
		track.AddBool ("Mute", 		!cur->isOn());
		track.AddInt16("OutputLine", cur->getRouteLine());
		track.AddInt16("MidiIn", 	 cur->GetMidiInCh());
		track.AddInt16("MidiEnale",  cur->IsMidiInEnable());

		BMessage settings;
		trackManager->SaveTrackSettings(cur, &settings);
		track.AddMessage("settings", &settings);
		
		BMessage patterns;
		for(int i=0; i<song->getNumberMeasure(); i++)
		{
			BMessage pattern;
		
			Pattern* pat = cur->getPatternAt(i); 
		
			for(int k=0;k<pat->getNumberNotes();k++)
			{
				BMessage note;
				note.AddBool( "Value", pat->getNoteAt(k)->getValue());
				note.AddFloat("Gain",  pat->getNoteAt(k)->Gain());
				note.AddFloat("Pan",   pat->getNoteAt(k)->Pan());
				note.AddInt16("Note",  pat->getNoteAt(k)->getNote());

				pattern.AddMessage("Note", &note);
			}		
		
			patterns.AddMessage("Pattern", &pattern);
		}
		
		track.AddMessage("Patterns",&patterns);

		tracksSettings.AddMessage("TrackSettings", &track);
	}

	songInfo.AddMessage("TracksSettings", &tracksSettings);
    
    
    // Playlist: 
	BMessage playlist;

	//TODO: move into Sequence class!!

	playlist.AddBool("Enable", song->getSequence()->loop_enable);
	playlist.AddInt16("Point", song->getSequence()->loop_points[0]);
	playlist.AddInt16("Point", song->getSequence()->loop_points[1]);
    	
	for(int p=0;p<song->getSequence()->getMaxSeq()+1;p++)
	{
		BMessage sequence;
		/*Inserire altro ciclo loop*/
		for(int16 jx=0;jx<song->getSequence()->getMaxPat();jx++)
		{
			int y = song->getSequence()->ItemAt(p,jx);
			if ( y >= 0 )
				sequence.AddInt16("Pattern", jx);
		}
		playlist.AddMessage("Sequence", &sequence);
	}
    	
    songInfo.AddMessage("Playlist", &playlist);
	songInfo.Flatten(file);	

    AddMime(file);
    	
//    file->Close();
    delete file;
	
	LogInfo("File Saved!\n");

	return B_OK;	
}
