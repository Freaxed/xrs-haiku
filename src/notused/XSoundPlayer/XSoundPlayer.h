#ifndef _XSOUNDPLAYER_H
#define _XSOUNDPLAYER_H

#include <MediaDefs.h>
#include <MediaNode.h>
#include "SoundProducer.h"


class BMediaRoster;
//class SoundProducer;

class XSoundPlayer
{
	public:
	enum sound_player_notification {
			B_STARTED = 1,
			B_STOPPED,
			B_SOUND_DONE
		};
	XSoundPlayer(	
				const media_raw_audio_format  format, 
				const char * name = NULL,
				SoundProcessFunc f=NULL,
				void (*Notifier)(void *, sound_player_notification what, ...) = NULL,
				void * cookie = NULL);
				
	~XSoundPlayer();/* destructor */
	
	status_t		InitCheck();
	void		Start();
	void		Stop();
	bigtime_t		Latency();	
	
	private:
	
		status_t 	StopPlaying();
		status_t	BreakPlayConnection();
		status_t	MakePlayConnection(const media_raw_audio_format & format);
		status_t	IPlayer(const char *name);
		
		status_t 	m_error;
		void*	m_cook;

		BMediaRoster * m_roster;
		media_node m_audioMixerNode;
		media_input m_audioMixerInput;
		media_output m_playOutput;
		
		bool	started;
		
	public:	
		SoundProducer * m_playNode;
		SoundProcessFunc PlayFile;
		//int	NotifyPlayFile;
/*static	void PlayFile(
				void * cookie, 
				bigtime_t timestamp, 
				void * data,
				size_t size, 
				const media_raw_audio_format & format){};
static	void NotifyPlayFile(
				void * cookie,
				int32 code,
				...){};*/
};
#endif
