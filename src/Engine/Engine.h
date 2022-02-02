/*
 * 
 * Copyright 2006-2022, Andrea Anzani.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

/*
	AudioEngine. 32bit 44100 2channels.
*/

#ifndef _Engine_H_
#define _Engine_H_

#include	<Looper.h>
#include	<MediaDefs.h>
#include	<String.h>
#include	<malloc.h>

//* audio definition
#define		_DATA_		float
#define		CHANNELS	2
#define		FREQUENCY	44100

//sound buffer engine
#define	 PREROLL_NUM	2
#define	 PREROLL_DIM	16384
#define	 FRAMES_NUM		1024
#define	 FRAMESIZE		8 //fullframe (with also the channels)
#define	 PREBUFFER_SIZE	2

//internal
#define	 MESSAGE_NEWBUF 	'nbuf'
#define  ENGINE_PRIORITY	B_URGENT_PRIORITY
//common
#define	 _ME_	__PRETTY_FUNCTION__


#define CHECK_LOCK if(!IsLocked()) debugger("Function called but not locked!");
#define IF_LOCK if(Lock()) {
#define UNLOCK Unlock(); }


class BSoundPlayer;

class Engine: public BLooper {

	public:
			 		Engine(const char* name="UntitledEngine");
					~Engine();
		
		virtual 	void			MessageReceived(BMessage* message);
					
		status_t 		Init();
		void			Start(); //start the song
		void			Stop();  //stop the song
		bool			IsPlaying();

		void			ReallyStop();  //stops the audio flow
		void			ReallyStart();// starts the audio flow
	
	public:
			status_t	Acquire(const char *who );
			status_t	Release(const char *who );		
			status_t	AcquireEtc(const char *who );
	
	protected: 
	
		virtual void	SecureProcessBuffer(void * buffer, size_t size) = 0;
		virtual void	Starting(){};
		virtual	void	Stopping(){};
		
		BSoundPlayer*			player;
	
	private:
	
	
			void			DoStart();
			void			DoStop();
		
			sem_id					stresaforo;	
			bool					isPlaying;
			bool					loop_enable;


									
			
			BString					fName;
	
	static	void FillBuffer(void * cookie,void * data,	size_t size,const media_raw_audio_format & format);
			void ProcessBuffer(void * data,	size_t size);

	//Pre-buffer system 
	
	 uint64		currentBuffer;
	 void*		buffers[PREBUFFER_SIZE];

	 void* 		_getBuffer();	 //get the current buffer
	 void		_changeBuffer(); //get the new choosen buffer
						
	 void		_prepare();
	 void		_finalize();
	 void		_clearBuffers();
			
};

#endif

