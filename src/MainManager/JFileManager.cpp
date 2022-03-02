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
#include	"JuiceEngine.h"

#include	"version.h"

#define 	CLEAR_TRS	'cls'
#define		DEL_TRS		'del'
#define 	DEL_TRACK 	'delt'



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
	song->getSequence()->all = songInfo.GetBool("PlayAll", false);
	
	song->setTempo	 (songInfo.GetInt32("BeatsPerMinute", 120));
	song->setBeatInfo(songInfo.GetInt16("NumBeats", 		4), 
	                  songInfo.GetInt16("BeatDivision", 	4));
	
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

	
void JFileManager::ExportWave(BMessage *info)
{
	if(!filepanel)
	filepanel=new BFilePanel(B_SAVE_PANEL,NULL,NULL,0,false,NULL,NULL,true);
	filepanel->Show();
	filepanel->SetMessage(info);
}


#include "MediaFileSaver.h"
#include "MeasureManager.h"
#include <algorithm>

status_t
JFileManager::RenderAsWave(BMessage *message, Song* song)
{

	// decoding BMessage
	entry_ref rif;	 
	if ( message->FindRef("directory", &rif) != B_OK ) 
	 	return B_ERROR;
	
	BEntry e(&rif);
	BPath  p(&e);

	p.Append(message->FindString("name"));

	LogTrace("Exporting to %s\n",p.Path());
	message->PrintToStream();

	size_t empty_space =  message->GetInt32("empty_space", 0) * 44100; //in frames

    if(filepanel!=NULL) 
	{
		delete filepanel;
		filepanel=NULL;
	}
	
	MediaFileSaver fileSaver;
	
	fileSaver.Open(p.Path());

	// 0 -> currentPattern
	// 1 -> allPatterns
	// see Panels::showExport

	bool playAllPatterns 	 = (message->GetInt32("pattern_index", 0) == 1);
	bool previousPatternMode = MeasureManager::Get()->GetPatternMode();
	
	
	// Let's really stop the engine.
	
	JuiceEngine::Get()->Stop();
	JuiceEngine::Get()->ReallyStop();
	MeasureManager::Get()->SetPatternMode(playAllPatterns);
	
	//Do stuff
	
	//create buffer
	void*	buffer = malloc(JuiceEngine::Get()->GetPreferredBufferSize());
	
	memset(buffer, 0, JuiceEngine::Get()->GetPreferredBufferSize());
	
	JuiceEngine::Get()->LockEngine   ("RenderAsWave");
	if (!playAllPatterns)
		JuiceEngine::Get()->SetLoopEnable(false);
	JuiceEngine::Get()->Start(); // Reset signal like when starting..
	JuiceEngine::Get()->UnlockEngine ("RenderAsWave");	
	
	//FIXME:
		//-> rendering full song is working but
		// not working: rendering full song + extra space at the end.
	
	
	
	//let's calculate how many samples we need to write to the file..
	int64 seqCount = playAllPatterns ? (song->getSequence()->getMaxSeq() + 1) : 1;

	LogTrace("Exporting %ld samples per beat , Number of beats: %d", JuiceEngine::Get()->GetSamplesPerBeat(), song->GetBeats());
	
	int64 totalSamples = (JuiceEngine::Get()->GetSamplesPerBeat() * song->GetBeats()) * seqCount;
	
	LogTrace("Exporting %ld patterns; %f seconds ; %f space", seqCount, (float)((float)totalSamples/44100.0f), (float)((float)empty_space/44100.0f));

	totalSamples += empty_space;

	while(totalSamples > 0)	// detect the end! 
	{
		JuiceEngine::Get()->SecureProcessBuffer(buffer, JuiceEngine::Get()->GetPreferredBufferSize()); // loop in calling SecureProcessBuffer
		fileSaver.WriteBlock((float*)buffer, std::min<int64>((int64)1024, totalSamples));
		totalSamples -= 1024;
	}

	
	fileSaver.Close();

	BNode node(p.Path());
	AddMime(&node, "audio/x-wave");


	JuiceEngine::Get()->LockEngine   ("RenderAsWave");
	JuiceEngine::Get()->Stop(); // Reset signal like when stopping..
	if (!playAllPatterns)
		JuiceEngine::Get()->SetLoopEnable(true);
	JuiceEngine::Get()->UnlockEngine ("RenderAsWave");

	MeasureManager::Get()->SetPatternMode(previousPatternMode);
	
	JuiceEngine::Get()->ReallyStart();

	free(buffer);
	return B_OK;
}

void
JFileManager::AddMime(BNode* file, const char* mime)
{
	BNodeInfo	info;
	info.SetTo(file);
	info.SetType(mime);
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
	songInfo.AddBool   ( "PlayAll"      , song->getSequence()->all);

	songInfo.AddInt32  ( "BeatsPerMinute", song->getTempo());
 	songInfo.AddInt16  ( "NumBeats"		 , song->GetBeats());
	songInfo.AddInt16  ( "BeatDivision"	 , song->GetBeatDivision());

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
