/*******************************************************************************
/
/	File:			SoundProducer.h
/
/   Description:	Produce sound to pass along to some sound-consuming Node.
/
/	Copyright 1998-1999, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#if !defined( _SoundProducer_h )
#define _SoundProducer_h

#include <BufferProducer.h>
#include "SoundUtils.h"

//	To use this Producer:
//	1. Create Play and Notify hooks, or subclass SoundProducer
//	   if you'd rather use the inheritance hierarchy.
//	   * The Play function should do whatever you want to do
//	     to fill the buffers you're going to send.
//	   * The Notify function should handle whatever events
//	     you wish to handle (defined in SoundUtil.h).
//	2: Create an instance of SoundProducer, giving it the
//	   appropriate hook functions. Or, create an instance of an
//	   appropriate subclass if you've made one.
//	3: Register your new Producer with the MediaRoster.
//	4: Connect your Producer to some Consumer.
//	5: Start or Stop the Producer to start or stop producing
//	   buffers.
//	   Seek the Producer to set the offset of the timestamps that
//	   your Play function will see.
//	6: When you're done, disconnect the Producer, then delete it.

class SoundProducer :
	public BBufferProducer
{
public:
		SoundProducer(
				const char * name,
				const media_raw_audio_format * format = NULL,
				SoundProcessFunc playFunc = NULL,
				SoundNotifyFunc notifyFunc = NULL,
				void * cookie = NULL);
		~SoundProducer();

		//	This function is OK to call from any thread.
		status_t SetHooks(
				SoundProcessFunc playFunc = NULL,
				SoundNotifyFunc notifyFunc = NULL,
				void * cookie = NULL);

		// This is our conception of where we are in
		// terms of playing sounds -- it's the last
		// media time of the last valid buffer we sent.
		bigtime_t CurrentMediaTime();
		bigtime_t Latency();
		
		//	The MediaNode interface
public:
virtual		port_id ControlPort() const;
virtual		BMediaAddOn* AddOn(
				int32 * internal_id) const;


protected:

virtual	void Start(
				bigtime_t performance_time);
virtual	void Stop(
				bigtime_t performance_time,
				bool immediate);
virtual	void Seek(
				bigtime_t media_time,
				bigtime_t performance_time);
virtual	void SetRunMode(
				run_mode mode);
virtual	void TimeWarp(
				bigtime_t at_real_time,
				bigtime_t to_performance_time);
virtual	void Preroll();
virtual	void SetTimeSource(
				BTimeSource * time_source);
virtual	status_t HandleMessage(
				int32 message,
				const void * data,
				size_t size);

		//	The BufferProducer interface
virtual	status_t FormatSuggestionRequested(
				media_type type,
				int32 quality,
				media_format * format);
virtual	status_t FormatProposal(
				const media_source & output,
				media_format * format);
virtual	status_t FormatChangeRequested(
				const media_source & source,
				const media_destination & destination,
				media_format * io_format,
				int32 * out_change_count);
virtual	status_t GetNextOutput(	/* cookie starts as 0 */
				int32 * cookie,
				media_output * out_output);
virtual	status_t DisposeOutputCookie(
				int32 cookie);
virtual	status_t SetBufferGroup(
				const media_source & for_source,
				BBufferGroup * group);
virtual	status_t VideoClippingChanged(
				const media_source & for_source,
				int16 num_shorts,
				int16 * clip_data,
				const media_video_display_info & display,
				int32 * out_from_change_count);
virtual	status_t GetLatency(
				bigtime_t * out_lantency);
virtual	status_t PrepareToConnect(
				const media_source & what,
				const media_destination & where,
				media_format * format,
				media_source * out_source,
				char * out_name);
virtual	void Connect(
				status_t error, 
				const media_source & source,
				const media_destination & destination,
				const media_format & format,
				char * io_name);
virtual	void Disconnect(
				const media_source & what,
				const media_destination & consumer);
virtual	void LateNoticeReceived(
				const media_source & what,
				bigtime_t how_much,
				bigtime_t performance_time);
virtual	void EnableOutput(
				const media_source & what,
				bool enabled,
				int32 * change_tag);

protected:
		//	Functions called when no hooks are installed.
		//	OK to override instead of installing hooks.
virtual	void Play(
				bigtime_t time,
				const void * data,
				size_t size,
				const media_raw_audio_format & format);
virtual	void Notify(
				int32 cause,
				...);

private:
		SoundProcessFunc m_playHook;
		SoundNotifyFunc m_notifyHook;
		void * m_cookie;
		media_output m_output;
		thread_id m_thread;
		port_id m_port;
		media_raw_audio_format m_raw_format;
		int64 m_frames_played;
		BBufferGroup * m_buffers;
		bigtime_t m_downstream_latency;
		bigtime_t m_private_latency;

		// State variables
		bool m_running;		// currently producing buffers
		bool m_muted;		// not producing buffers because
							// we were told to shut up
		bool m_starting;	// a Start is pending
		bool m_stopping;	// a Stop is pending
		bool m_seeking;		// a Seek is pending

		// The "queue" of start, stop, and seek events
		// that we'll be handling. We don't currently
		// handle time warps.
		// My notation for times: tr = real time,
		// tp = performance time, tm = media time.
		bigtime_t m_tpStart;	// when we Start
		bigtime_t m_tpStop;		// when we Stop
		bigtime_t m_tpSeekAt;	// when we Seek
		bigtime_t m_tmSeekTo;	// target time for Seek
		
		// The transformation from media to peformance time.
		// d = p - m, so m + d = p.
		bigtime_t m_delta;

		// ProcessingLatency is the time it takes to process
		// a buffer. Override it if you're doing something
		// time-intensive in the play hook function.
virtual bigtime_t ProcessingLatency();

		void alloc_buffers();
		bigtime_t TotalLatency();

		// The actual thread doing the work.
static		status_t ThreadEntry(
				void * obj);
		void ServiceThread();
		void DoHookChange(
				void * msg);
};

#endif /* _SoundProducer_h */
