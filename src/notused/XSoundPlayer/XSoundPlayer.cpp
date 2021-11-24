#include "XSoundPlayer.h"
#include "SoundProducer.h"
#include <MediaRoster.h>
#include <TimeSource.h>
#include <assert.h>
#include <stdio.h>
#define FPRINTF(args) fprintf args
#define CONNECT FPRINTF

XSoundPlayer::XSoundPlayer(	
				const media_raw_audio_format  format,
				const char * name = NULL,
				SoundProcessFunc f = NULL,
				void (*Notifier)(void *, sound_player_notification what, ...) = NULL,
				void * cookie = NULL)
{
			started=false;
			PlayFile=f;
			m_cook=cookie;
			m_error=IPlayer(name);
			if(m_error==B_OK)
			m_error=MakePlayConnection(format);
		
			
}
XSoundPlayer::~XSoundPlayer(){

	/* here maybe just a good disconnect?? and cleanup */

	StopPlaying(); 	

}

status_t XSoundPlayer::InitCheck(){ return m_error;}
void XSoundPlayer::Start(){
	
	if(m_error!=B_OK || started ) return;
	
	bigtime_t then = m_playNode->TimeSource()->Now()+50000LL;
	m_roster->StartNode(m_playNode->Node(), then);
	
	started=true;

}
void XSoundPlayer::Stop(){ 
	if(m_error!=B_OK || !started) return;
	
	m_roster->StopNode(m_playNode->Node(), m_playNode->TimeSource()->Now(),true);
	started=false;
	
}
bigtime_t 
XSoundPlayer::Latency(){
	return m_playNode->Latency();
}


status_t
XSoundPlayer::MakePlayConnection(const media_raw_audio_format & format)
{
	//	Find an available input for the mixer node.
	int32 count = 0;
	status_t err = m_roster->GetFreeInputsFor(m_audioMixerNode, &m_audioMixerInput, 1, &count, B_MEDIA_RAW_AUDIO);
	if (err < B_OK) {
		CONNECT((stderr, "CaptureWindow::MakePlayConnection(): no free mixer inputs\n"));
		return err;
	}
	if (count < 1) {
		return B_BUSY;
	}

	//	Find an available output for our own Node. Note that we go through the
	//	MediaRoster; calling Media Kit methods directly on Nodes in our app is
	//	not OK (because synchronization happens in the service thread, not in
	//	the calling thread).
	// TODO: explain this last bit
	err = m_roster->GetFreeOutputsFor(m_playNode->Node(), &m_playOutput, 1, &count, B_MEDIA_RAW_AUDIO);
	if (err < B_OK) {
		CONNECT((stderr, "CaptureWindow::MakePlayConnection(): no free play node outputs\n"));
		return err;
	}
	if (count < 1) {
		return B_BUSY;
	}

	//	Find out what the preffered (DAC) time source is.
	media_node tsnode;
	err = m_roster->GetTimeSource(&tsnode);
	if (err < B_OK) {
		CONNECT((stderr, "CaptureWindow::MakePlayConnection(): no default time source: using system time source\n"));
		m_roster->GetSystemTimeSource(&tsnode);
	}

	//	Apply the time source in effect to our own Node.
	err = m_roster->SetTimeSourceFor(m_playNode->ID(), tsnode.node);
	if (err < B_OK) {
		CONNECT((stderr, "CaptureWindow::MakePlayConnection(): set time source for play node failed\n"));
		return err;
	}

	// Create a media format with the raw audio format we were given.
	media_format fmt;
	fmt.u.raw_audio=(const  media_multi_audio_format &) format;
	fmt.type = B_MEDIA_RAW_AUDIO;

	//	Tell the producer where we want data to go.
	err = m_playNode->SetHooks(PlayFile, NULL, m_cook);
	if (err < B_OK) {
		CONNECT((stderr, "CaptureWindow::MakePlayConnection(): set hooks for play node failed\n"));
		return err;
	}

	//	Using the same structs for input and output is OK in BMediaRoster::Connect().
	err = m_roster->Connect(m_playOutput.source, m_audioMixerInput.destination, &fmt, &m_playOutput, &m_audioMixerInput);
	if (err < B_OK) {
		CONNECT((stderr, "CaptureWindow::MakePlayConnection(): failed to connect play node to mixer\n"));
		m_playNode->SetHooks(0, 0, 0);
		return err;
	}

	return B_OK;
}

status_t
XSoundPlayer::BreakPlayConnection()
{
	CONNECT((stderr, "Breaking play connection...\n"));
	status_t err;

	if (m_playOutput.source == media_source::null
		|| m_audioMixerInput.destination == media_destination::null)
	{
		// someone's already done our dirty work
		CONNECT((stderr, "Play connection already broken.\n"));
		return B_OK;
	}

	// Stop the node so that it's no longer sending buffers,
	// then disconnect it from the mixer.
	CONNECT((stderr, "Stopping node...\n"));
	err = m_roster->StopNode(m_playNode->Node(), m_playNode->TimeSource()->Now(),
		true);
	CONNECT((stderr, "Disconnecting node...\n"));
	err = m_roster->Disconnect(m_playOutput.node.node, m_playOutput.source,
		m_audioMixerInput.node.node, m_audioMixerInput.destination);
	CONNECT((stderr, "Play connection broken.\n"));
	m_playOutput.source = media_source::null;
	m_audioMixerInput.destination = media_destination::null;
	return err;
}

status_t
XSoundPlayer::StopPlaying()
{
	BreakPlayConnection();
	m_playNode->SetHooks(0, 0, 0);
	return B_OK;	
}

status_t
XSoundPlayer::IPlayer(const char *name)
{
	
	status_t error;
	/*int max_input_count = 64;
	live_node_info * lni = new live_node_info[max_input_count];
	array_delete<live_node_info> lni_delete(lni);	//	auto-delete[]*/
	
	
				
		//	Make sure the media roster is there (which means the server is there).
		m_roster = BMediaRoster::Roster(&error);
		if (!m_roster) {
			return error;
		}

		
		error = m_roster->GetAudioMixer(&m_audioMixerNode);
		if (error < B_OK) {	//	there's no mixer?
			return error;
		}

		// Create our internal Node which plays sound, and register it.
		m_playNode = new SoundProducer(name);
		error = m_roster->RegisterNode(m_playNode);
		if (error < B_OK) {
			m_playNode->Release();
			return error;
		}

	
	
	return B_OK;

}
