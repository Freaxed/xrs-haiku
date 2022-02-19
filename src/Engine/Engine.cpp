/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "Engine.h"
#include "Log.h"

#include <SoundPlayer.h>
#include <Messenger.h>

#define	 MESSAGE_START 	'star'
#define	 MESSAGE_STOP 	'stop'

Engine::Engine(const char* name) :
	BLooper("Engine",ENGINE_PRIORITY),fName(name) {	

	isPlaying = false;
	
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
	fmt.buffer_size = GetPreferredBufferSize();
	
	
	player = new BSoundPlayer(&fmt,fName.String(),FillBuffer,NULL,this);
	
	player->SetHasData(true); //in haiku it must be set
	
	err = player->InitCheck();
	
	if (err != B_OK) 
		LogFatal("Error creating the player!\n");
	
	ReallyStart();
	
	return err;
}


Engine::~Engine() {

	ReallyStop();
	delete player;
	_finalize();
}

void
Engine::ReallyStop() {
	
	IF_LOCK
	
	player->SetHasData(false);
	player->Stop();
	
	_clearBuffers();
	
	UNLOCK
}

void
Engine::ReallyStart(){
	
	IF_LOCK
	
	player->SetHasData(true);
	player->Start();
	
	UNLOCK
}

bool 
Engine::IsPlaying(){ return isPlaying; }

void Engine::Start() {
	LogTrace("Engine::Start %d vs %d", find_thread(NULL) , BLooper::Thread());
	// this must be sync! to keep the real meaning of start!
	// if the caller is the Engine itself (this means the looper is locked) we direct do the start.
	if (find_thread(NULL) != Thread() && !IsLocked())
	{
		BMessage reply;
		BMessenger(this).SendMessage(MESSAGE_START, &reply);
		//reply.PrintToStream();
	}
	else
	{
		//We are already locked!
		DoStart();
	}
}

void Engine::Stop() {
	LogTrace("Engine::Stop caller %d vs BLooper %d", find_thread(NULL) , BLooper::Thread());
	// this must be sync! to keep the real meaning of stop!
	// if the caller is the Engine itself (this means the looper is locked) we direct do the stop.
	if (find_thread(NULL) != Thread() && !IsLocked())
	{
		BMessage reply;
		BMessenger(this).SendMessage(MESSAGE_STOP, &reply);
		//reply.PrintToStream();
	}
	else
	{
		//We are already locked!
		DoStop();
	}

}

void
Engine::DoStart() 
{
	CHECK_LOCK
			
	if(isPlaying) 
	{
		Stop(); 
		return; 
	}
	
   	isPlaying = true;

	Starting();
}



void
Engine::DoStop()
{
	CHECK_LOCK
	
	if(!isPlaying) return;

	isPlaying = false;

	Stopping();
}

bool 
Engine::LockEngine(const char *who) {
	LogTrace("Engine::Acquire by %s (%d) - prev %s",who, find_thread(NULL), prevLocker.String());
	bool l = Lock();
	if (!l)
		LogTrace("Engine::Acquire can't lock!");
	else
		prevLocker = who;
	return l;
}

bool 
Engine::UnlockEngine(const char *who) {
	bool rel = IsLocked();	
	if (rel) {
		Unlock();
	}
	LogTrace("Engine::Release by %s (%d) (%s)",who, find_thread(NULL), rel ? "ok" : "was not locked!");
	return true;
}

// status_t 
// Engine::AcquireEtc(const char *who) {
// 	LogTrace("Engine::AcquireEtc by %s (%d)",who, find_thread(NULL));
// 	LockWithTimeout(0);
// 	return IsLocked() ? B_OK : B_ERROR;
// }

// void 
// Engine::FillBuffer(void * theCookie, void *buffer, size_t size, const media_raw_audio_format & format) 
// {
// 	Engine *engine = (Engine*)theCookie;
// 	if (engine->LockEngine("Buf")){
// 		engine->SecureProcessBuffer(buffer, size);
// 		LogTrace("Engine::Release");
// 		engine->UnlockEngine("Buf");
// 	}
// }

void 
Engine::FillBuffer(void * theCookie,void * buffer,size_t size,const media_raw_audio_format & format) 
{
	Engine *engine = (Engine*)theCookie;

	void* buf = engine->_getBuffer();
		
	memset(buffer, 0, size);

	engine->PostMessage(MESSAGE_NEWBUF);

	memcpy(buffer, buf, size);
}

void
Engine::MessageReceived(BMessage* message)
{
	switch(message->what) 	{
	
		case MESSAGE_NEWBUF:
			_changeBuffer();		
			SecureProcessBuffer(_getBuffer(), GetPreferredBufferSize());
			break;
		case MESSAGE_START:
			DoStart();
		break;
		case MESSAGE_STOP:
			DoStop();
			message->SendReply('okok');
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

		buffers[i]=malloc(GetPreferredBufferSize());
		memset(buffers[i],0,GetPreferredBufferSize());
	}
}

void* 
Engine::_getBuffer(){
	return buffers[currentBuffer];
}

void	
Engine::_changeBuffer()
{
	if(currentBuffer == PREBUFFER_SIZE-1) 
		currentBuffer=0;
	else
		currentBuffer++;
}
