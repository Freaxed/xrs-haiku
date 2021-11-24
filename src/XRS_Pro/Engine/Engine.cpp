/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Engine.h"
#include "Logger.h"

#include <SoundPlayer.h>

Engine::Engine(const char* name) :
	BLooper("AudioEngine",ENGINE_PRIORITY),fName(name) {	

	isPlaying=false;

	stresaforo=create_sem(1, "audio_engine_sem");
	
	Engine::_prepare();
	Run();
}

status_t
Engine::Init() {
	
	status_t err;
	
	media_raw_audio_format	fmt;
		
	fmt.format = media_raw_audio_format::B_AUDIO_FLOAT;
	fmt.channel_count = 2;
	fmt.frame_rate = 44100;
	fmt.byte_order = B_MEDIA_LITTLE_ENDIAN;
	fmt.buffer_size = FRAMES_NUM*FRAMESIZE;
	
	
	player = new BSoundPlayer(&fmt,fName.String(),FillBuffer,NULL,this);
	
	player->SetHasData(true); //in haiku it must be set
	
	err = player->InitCheck();
	
	if (err != B_OK) 
		Log(LOG_FATAL,"Error creating the player!\n");
	
	ReallyStart();
	
	return err;
}


Engine::~Engine() {
	
	
	if(AcquireEtc(_ME_) != B_OK)
			debugger("~Juice must be loked!");
			
	ReallyStop();

	delete_sem(stresaforo);
	
	delete player;
		
	_finalize();
}

void
Engine::ReallyStop() {
	player->Stop();
	_clearBuffers();
}

void
Engine::ReallyStart(){
	player->Start();
}

bool 
Engine::IsPlaying(){ return isPlaying; }

void
Engine::Start() {	
		
		if(isPlaying) 
		{ 
			Stop(); 
			return; 
		}
		
		Acquire(_ME_);
		///Quick Dirty
    	isPlaying=true;
		Release(_ME_);
		Starting();
}

void
Engine::Stop() {

	if(!isPlaying) return;
	Acquire(_ME_);
	 isPlaying=false;
	Release(_ME_);
	Stopping();
}

status_t 
Engine::Acquire(const char *who) {
	//Log(LOG_INFO,"Acquire:%s by %ld",who, find_thread(NULL));
	acquire_sem(stresaforo);
	return B_OK;
}

status_t 
Engine::Release(const char *who) {
	//Log(LOG_INFO,"Release:%s by %ld",who, find_thread(NULL));
	release_sem(stresaforo);
	return B_OK;
}

status_t 
Engine::AcquireEtc(const char *who) {
	//Log(LOG_INFO,who);
	acquire_sem_etc(stresaforo,1,B_RELATIVE_TIMEOUT,0);
	return B_OK;
}

void 
Engine::FillBuffer(void * theCookie,void * buffer,size_t size,const media_raw_audio_format & format) {

	Engine *engine=(Engine*)theCookie;

	void* buf=engine->_getBuffer();
		
	memset(buffer,0,size);
	
	engine->PostMessage(MESSAGE_NEWBUF);
	memcpy(buffer,buf,size);
  
}


void
Engine::ProcessBuffer(
	void * buffer,
	size_t size)
{
		
	if(AcquireEtc(_ME_) == B_OK)
	{
		SecureProcessBuffer(buffer,size);
		Release(_ME_);
	}
	else 
	{  //sem locked!
		memset(buffer,0,size);
		Log(LOG_WARN,"Lost packet!");
	}
	
	//Log(LOG_WARN,"Latency: %d",player->Latency());
		
}

void
Engine::MessageReceived(BMessage* message)
{

	switch(message->what) 	{
	
		case MESSAGE_NEWBUF:
			_changeBuffer();		
			ProcessBuffer(_getBuffer(), FRAMES_NUM*FRAMESIZE);
			break;
		
		default:
			BLooper::MessageReceived(message);
			break;
	}
}


/**/
void
Engine::_prepare() {
	currentBuffer=0; //?
	_clearBuffers();
}

void
Engine::_finalize() {
	currentBuffer=0; //?
	for(int i=0;i<PREBUFFER_SIZE;i++) free(buffers[i]);
}

void
Engine::_clearBuffers(){

	for(int i=0;i<PREBUFFER_SIZE;i++){

		buffers[i]=malloc(FRAMES_NUM*FRAMESIZE);
		memset(buffers[i],0,FRAMES_NUM*FRAMESIZE);
	}
}

void* 
Engine::_getBuffer(){
	return buffers[currentBuffer];
}

void	
Engine::_changeBuffer()
{
	if(currentBuffer==PREBUFFER_SIZE-1) 
		currentBuffer=0;
	else
		currentBuffer++;
}
