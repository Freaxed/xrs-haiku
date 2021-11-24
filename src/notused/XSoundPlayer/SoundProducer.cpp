/*******************************************************************************
/
/	File:			SoundProducer.cpp
/
/   Description:	Produce sound to pass along to some sound-consuming Node.
/
/	Copyright 1998-1999, Be Incorporated, All Rights Reserved
/
*******************************************************************************/

#include <TimeSource.h>
#include <BufferGroup.h>
#include <Buffer.h>
#include <Autolock.h>
#include <Debug.h>

#include <scheduler.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array_delete.h"
#include "SoundPrivate.h"
#include "SoundProducer.h"

#define NDEBUG 1
//	Compiling with NDEBUG means "release." By contrast, DEBUG turns on
//	extra debugging in some programs.
#if !NDEBUG
#define FPRINTF(args) fprintf args
#else
#define FPRINTF
#endif

//	Comment out the FPRINTF part of these lines to reduce verbiage.
#define WARNING //FPRINTF
#define LATE //FPRINTF
#define TRANSPORT //FPRINTF
#define NODE //FPRINTF
#define TIMING //FPRINTF
#define FORMAT //FPRINTF

//	These constants set a reasonable default format for us to use.
#define PREFERRED_SAMPLE_FORMAT media_raw_audio_format::B_AUDIO_FLOAT
#define PREFERRED_FRAME_RATE 	44100
#if B_HOST_IS_BENDIAN
 #define PREFERRED_BYTE_ORDER B_MEDIA_BIG_ENDIAN
#else
 #define PREFERRED_BYTE_ORDER B_MEDIA_LITTLE_ENDIAN
#endif
#define PREFERRED_CHANNEL_COUNT 	2
#define PREFERRED_BUFFER_SIZE 	4096*2;

SoundProducer::SoundProducer(
	const char * name,
	const media_raw_audio_format * format,
	SoundProcessFunc playFunc,
	SoundNotifyFunc notifyFunc,
	void * cookie) :
	BMediaNode(name ? name : "SoundProducer"), 
	BBufferProducer(B_MEDIA_RAW_AUDIO)
{
	NODE((stderr, "SoundProducer::SoundProducer(%p, %p, %p, %p)\n",
		name, playFunc, notifyFunc, cookie));

	if (!name) name = "SoundProducer";




	// Set up the hook functions.
	m_playHook = playFunc;
	m_notifyHook = notifyFunc;
	m_cookie = cookie;

	// Create the port that we publish as our ControlPort.
	// As a simple producer, our queue length doesn't need
	// to be very long.
	char pname[32];
	sprintf(pname, "%.20s Control", name);
	m_port = create_port(3, pname);

	// m_raw_format is the format we've been told to use;
	// i.e. the format we'd like to use. The object
	// media_raw_audio_format::wildcard is a format
	// structure whose fields are all unitialized.
	m_raw_format = media_raw_audio_format::wildcard;
	if (format != 0) {
		m_raw_format = *format;
	}

	// If anything has been left unspecified in the format
	// (== media_raw_audio_format::wildcard.*), or if any
	// field is invalid (< media_raw_audio_format::wildcard.*),
	// fill it in with a reasonable default.
	if (m_raw_format.frame_rate <= media_raw_audio_format::wildcard.frame_rate) {
		m_raw_format.frame_rate = PREFERRED_FRAME_RATE;
	}
	if (m_raw_format.format <= media_raw_audio_format::wildcard.format) {
		m_raw_format.format = PREFERRED_SAMPLE_FORMAT;
	}
	if (m_raw_format.byte_order <= media_raw_audio_format::wildcard.byte_order) {
		m_raw_format.byte_order = PREFERRED_BYTE_ORDER;
	}
	if (m_raw_format.channel_count <= media_raw_audio_format::wildcard.channel_count) {
		m_raw_format.channel_count = PREFERRED_CHANNEL_COUNT;
	}
	if (m_raw_format.buffer_size <= media_raw_audio_format::wildcard.buffer_size) {
		m_raw_format.buffer_size = PREFERRED_BUFFER_SIZE;
	}

	// Initialize our single media_output, and use
	// FormatSuggestionRequested to stuff the output's
	// format fields with our raw audio format information.
	// m_output represents the actual connection, so
	// m_output.format.u.raw_audio will eventually be
	// set to the *acutal* format we end up using, as
	// opposed to m_raw_format, which is our preferred
	// format.
	m_output.format.type = B_MEDIA_RAW_AUDIO;
	m_output.format.u.raw_audio = media_raw_audio_format::wildcard;
	FormatSuggestionRequested(B_MEDIA_RAW_AUDIO, 0, &m_output.format);

	// Finish specifying the media_output. Make sure
	// it knows the control port associated with the
	// source, and the index of the source (since we
	// only have one, that's trivial).
	m_output.source.port = m_port;
	m_output.source.id = 1;
	sprintf(m_output.name, "%.20s Output", name);

	// Set up the timing variables that we'll be using.
	m_frames_played = 0;
	// For downstream latency, we'll fill in a real value
	// when we're connected.
	m_downstream_latency = 15000;
	// For our own latency, we'll take a guess at the 
	// scheduling latency and multiply by a fudge
	// factor.
	m_private_latency = estimate_max_scheduling_latency(m_thread)*2;
	
	m_tpStart = 0;
	m_tpStop = 0;
	m_tpSeekAt = 0;
	m_tmSeekTo = 0;
	m_delta = 0;
	m_running = false;
	m_starting = false;
	m_stopping = false;
	m_seeking = false;
	m_muted = false;
	m_buffers = 0;
#if !NDEBUG
	char fm3[100];
	string_for_format(m_output.format, fm3, 100);
	FORMAT((stderr, "SoundProducer format: %s\n", fm3));
#endif
//	fprintf(stderr,"FFFFFF\n");

	// Create and start the thread that we use to do all the work;
	// i.e. service the Control Port and produce the buffers.
	// Another way to do this would be to break out the control
	// and production functionality out into two new threads.
	m_thread = spawn_thread(ThreadEntry, "SoundProducer Thread", 
		suggest_thread_priority(B_AUDIO_PLAYBACK, 100, 1000, 100), this);
	resume_thread(m_thread);
}

bigtime_t 
SoundProducer::Latency(){
	bigtime_t late;
	if(GetLatency(&late)!=B_OK) late=0;
	return late;
}

SoundProducer::~SoundProducer()
{
	NODE((stderr, "SoundProducer::~SoundProducer()\n"));

	// Tell our thread that it's bedtime.
	write_port(m_port, MSG_QUIT_NOW, 0, 0);
	status_t s;
	while (wait_for_thread(m_thread, &s) == B_INTERRUPTED)
		NODE((stderr, "wait_for_thread() B_INTERRUPTED\n"));
	
	NODE((stderr, "SoundProducer::~SoundProducer(): thread %ld completed\n", m_thread));
	delete_port(m_port);
}

status_t
SoundProducer::SetHooks(
	SoundProcessFunc playFunc,
	SoundNotifyFunc notifyFunc,
	void * cookie)
{
	//	SetHooks needs to be synchronized with the service thread, else we may
	//	call the wrong hook function with the wrong cookie, which would be bad.
	//	Rather than do locking, which is expensive, we streamline the process
	//	by sending our service thread a request to change the hooks, and waiting
	//	for the acknowledge.
	status_t err = B_OK;
	set_hooks_q cmd;
	cmd.process = playFunc;
	cmd.notify = notifyFunc;
	cmd.cookie = cookie;
	//	If we're not in the service thread, we need to round-trip a message.
	if (find_thread(0) != m_thread) {
		cmd.reply = create_port(1, "SetHooks reply");
		//	Send the private message to our service thread.
		err = write_port(ControlPort(), MSG_CHANGE_HOOKS, &cmd, sizeof(cmd));
		if (err >= 0) {
			int32 code;
			//	Wait for acknowledge from the service thread.
			err = read_port_etc(cmd.reply, &code, 0, 0, B_TIMEOUT, 6000000LL);
			if (err > 0) err = 0;
			NODE((stderr, "SoundConsumer::SetHooks read reply: %#010lx\n", err));
		}
		//	Clean up.
		delete_port(cmd.reply);
	}
	else {
		//	Within the service thread, it's OK to just go ahead and do the change.
		DoHookChange(&cmd);
	}
	return err;
}





////////////////////////////////////////////////////////////////////////////////
//
//	BMediaNode-derived methods
//
////////////////////////////////////////////////////////////////////////////////



port_id SoundProducer::ControlPort() const
{
	return m_port;
}


BMediaAddOn* SoundProducer::AddOn(
	int32 * internal_id) const
{
	//	This object is instantiated inside an application.
	//	Therefore, it has no add-on.
	if (internal_id) *internal_id = 0;
	return 0;
}


void SoundProducer::Start(
	bigtime_t performance_time)
{
	// As a producer, it's important for us to handle Start
	// correctly. We should not start producing buffers until
	// the specified performance time has been reached.
	
	// We only handle Start if we're not already running,
	// and if there are no Stop requests pending. This
	// gives us a well-defined state where we cannot be
	// starting and stopping at the same time. The logic
	// below handles race conditions where we may have
	// come to a stop, but m_stopping has not yet been
	// set to false.
	if (!m_stopping || performance_time > m_tpStop) {
		if (!m_running || m_stopping) {
			// It's okay to handle this Start request.
			if (m_notifyHook) {
				(*m_notifyHook)(m_cookie, B_WILL_START, performance_time);
			}
			else {
				Notify(B_WILL_START, performance_time);
			}
			
			// We queue up the time at which we want
			// the start to take effect, and let our
			// service thread know that a Start request
			// is pending.
			m_tpStart = performance_time;
			m_starting = true;
			TRANSPORT((stderr, "SoundProducer start at %.4f (now %.4f)\n",
				us_to_s(m_tpStart), us_to_s(TimeSource()->Now())));
		}
	}
}


void SoundProducer::Stop(
	bigtime_t performance_time,
	bool immediate)
{
	// As a producer, it's important for us to handle Stop
	// correctly. Generally, we should not stop until
	// the given performance time, except when immediate =
	// true, in which case we need to stop producing sound
	// immediately (w/o deadlocking ourselves).

	bool notified = false;

	// The logic here is symmetric to the logic of Start.
	// We only handle Stop if we are already running,
	// or there are no Start requests pending. We also
	// handle the race condition where we have already
	// started but m_starting has not yet been set to false.
	if (!m_starting || performance_time > m_tpStart) {
		if (m_running || m_starting) {
			// It's okay to handle this Stop request.
			if (m_notifyHook) {
				(*m_notifyHook)(m_cookie, B_WILL_STOP, performance_time,
					immediate);
			}
			else {
				Notify(B_WILL_STOP, performance_time, immediate);
			}
			// Like Start, we queue up the time at which
			// we want the event to take effect, and let
			// our service thread know that a Stop
			// request is pending.
			notified = true;
			m_tpStop = performance_time;
			m_stopping = true;
			TRANSPORT((stderr, "SoundProducer stop at %.4f (now %.4f)\n",
				us_to_s(m_tpStop), us_to_s(TimeSource()->Now())));
		}
	}

	// If immediate = true, we must make the sound stop
	// right away, and we must not return from this
	// function until it does stop (i.e. until we can
	// guarantee that we won't be producing any more
	// buffers). We can't wait for the service thread's
	// main loop to handle to the Stop request before
	// returning because there's a good chance that we
	// ARE the service node, responding to a message
	// sent by the Media Roster -- and waiting on
	// ourselves would cause a deadlock. So, we do a
	// shortcut, and stop running immediately -- the
	// service thread's main loop will stop sending
	// buffers immediately, and will eat the stop
	// request later on.
	if (immediate) {
		if (! notified) {
			if (m_notifyHook) {
				(*m_notifyHook)(m_cookie, B_WILL_STOP, performance_time,
					immediate);
			}
			else {
				Notify(B_WILL_STOP, performance_time, immediate);
			}
		}
		m_running = false;
	}
}


void SoundProducer::Seek(
	bigtime_t media_time,
	bigtime_t performance_time)
{
	// Seek sets the "media time" of a node. What the
	// media time is interpreted to be varies from node
	// to node. For a Producer that plays a sound, media
	// time is generally interpreted to be the time	
	// offset of the media being played (where 0 is
	// the beginning of the media).
	if (m_notifyHook) {
		(*m_notifyHook)(m_cookie, B_WILL_SEEK, performance_time, media_time);
	}
	else {
		Notify(B_WILL_SEEK, performance_time, media_time);
	}
	m_tmSeekTo = media_time;
	TRANSPORT((stderr, "SEEK: Setting m_tmSeekTo to %.4f @ %.4f\n",
		us_to_s(m_tmSeekTo), us_to_s(performance_time)));
	m_tpSeekAt = performance_time;
	m_seeking = true;
	TIMING((stderr, "SoundProducer seek at %.4f (now %.4f)\n", us_to_s(m_tpSeekAt),
		us_to_s(TimeSource()->Now())));
}

void SoundProducer::SetRunMode(
	run_mode mode)
{
	if (mode == BMediaNode::B_OFFLINE) {
		// BMediaNode::B_OFFLINE means we don't need to run in
		// real time. So, we shouldn't run as a real time
		// thread.
		int32 new_prio = suggest_thread_priority(B_OFFLINE_PROCESSING);
		set_thread_priority(m_thread, new_prio);
	}
	else {
		//	We're running in real time, so we'd better have
		//	a big enough thread priority to handle it!
		//	Here's where those magic scheduler values
		//	come from:
		//
		//	* In the worst case, we process one buffer per
		//	  reschedule (we get rescheduled when we go to
		//	  look for a message on our Control Port), so
		//	  in order to keep up buffer production,
		//	  the duration of one buffer becomes our
		//	  scheduling period. If we don't know anything
		//	  about the buffers, we pick a reasonable
		//	  default.
		//	* We're a producer, so we should be a bit picky
		//	  about the jitter. 1ms seems like a reasonable
		//	  number.
		//	* The amount of time we spend processing is
		//	  our ProcessingLatency().
		bigtime_t period = 10000;
		if (buffer_duration(m_output.format.u.raw_audio) > 0) {
			period = buffer_duration(m_output.format.u.raw_audio);
		}
		int32 new_prio = suggest_thread_priority(B_AUDIO_PLAYBACK,
			period, 1000, ProcessingLatency());
		set_thread_priority(m_thread, new_prio);
	}
}


void SoundProducer::TimeWarp(
	bigtime_t at_real_time,
	bigtime_t to_performance_time)
{
	// Not implemented for now -- future versions will handle this
	// correctly.
	if (m_notifyHook) {
		(*m_notifyHook)(m_cookie, B_WILL_TIMEWARP, at_real_time, to_performance_time);
	}
	else {
		Notify(B_WILL_TIMEWARP, at_real_time, to_performance_time);
	}
}


void SoundProducer::Preroll()
{
	// Not implemented for now, and for a simple node like this one,
	// a Preroll probably isn't necessary. Future versions, however,
	// may do something here.
}


void SoundProducer::SetTimeSource(
	BTimeSource * /* time_source */)
{
}


status_t SoundProducer::HandleMessage(
	int32 message,
	const void * data,
	size_t size)
{
	// Check with each of our superclasses to see if they
	// understand the message. If none of them do, call
	// BMediaNode::HandleBadMessage().
	if (BMediaNode::HandleMessage(message, data, size) && 
		BBufferProducer::HandleMessage(message, data, size)) {
		BMediaNode::HandleBadMessage(message, data, size);
		return B_ERROR;
	}
	return B_OK;
}





////////////////////////////////////////////////////////////////////////////////
//
//	BMediaNode-derived methods
//
////////////////////////////////////////////////////////////////////////////////



status_t SoundProducer::FormatSuggestionRequested(
	media_type type,
	int32 /* quality */,
	media_format * format)
{
	// We're being asked what format we'd like to use.
	// We'll insist on a type of B_MEDIA_RAW_AUDIO,
	// and will suggest frame rate, channel count,
	// and raw audio format fields based on the
	// raw audio format passed into our constructor.
	// We don't bother to set endianness, since the
	// consumer will probably insist on host
	// endianness. Fine by us!
	NODE((stderr, "SoundProducer::FormatSuggestionRequested()\n"));

	if (type <= 0) type = B_MEDIA_RAW_AUDIO;
	if (type != B_MEDIA_RAW_AUDIO) return B_MEDIA_BAD_FORMAT;
	format->type = type;
	format->u.raw_audio = media_raw_audio_format::wildcard;
	format->u.raw_audio.frame_rate = m_raw_format.frame_rate;
	format->u.raw_audio.channel_count = m_raw_format.channel_count;
	format->u.raw_audio.format = m_raw_format.format;
	format->u.raw_audio.buffer_size = m_raw_format.buffer_size;
#if !NDEBUG
	char fmt[100];
	string_for_format(*format, fmt, 100);
	FORMAT((stderr, "return format %s\n", fmt));
#endif
	return B_OK;
}


status_t SoundProducer::FormatProposal(
	const media_source & output,
	media_format * format)
{
	if (output != m_output.source) {
		// We only accept proposals aimed at our single output.
		NODE((stderr, "SoundProducer::FormatProposal(): bad source\n"));
		return B_MEDIA_BAD_SOURCE;
	}
	if (format->type <= 0) {
		// the requestor doesn't even know what kind of media
		// we like! "Tell 'em, Ray -- "
		FormatSuggestionRequested(B_MEDIA_RAW_AUDIO, 0, format);
	}
	else {
		if (format->type != B_MEDIA_RAW_AUDIO) {
			// We really must insist that the format
			// be raw audio, so we cry bloody murder.
			goto err;
		}

		// Now, we're being asked if we'll support the
		// specified raw audio format.
		// 
		// We'll insist on the number of channels in
		// our preffered raw audio format. We'll
		// continue to suggest our favorite raw audio
		// format and buffer size if prompted by
		// wildcards, and maintain total apathy towards
		// endianness.

		if (format->u.raw_audio.format > media_raw_audio_format::wildcard.format) {
			m_output.format.u.raw_audio.format = format->u.raw_audio.format;
		}
		if (format->u.raw_audio.channel_count <= media_raw_audio_format::wildcard.channel_count) {
			format->u.raw_audio.channel_count = m_raw_format.channel_count;
		}
		else if (format->u.raw_audio.channel_count != m_raw_format.channel_count) {
			goto err;
		}
		if (format->u.raw_audio.format <= media_raw_audio_format::wildcard.format) {
			format->u.raw_audio.format = m_raw_format.format;
		}
		//	we don't care about byte order because we can handle both
		if (format->u.raw_audio.buffer_size < 1) {
			format->u.raw_audio.buffer_size = m_raw_format.buffer_size;
		}
	}
	format->u.raw_audio.byte_order = media_raw_audio_format::wildcard.byte_order;
#if !NDEBUG
	char fmt[100];
	string_for_format(*format, fmt, 100);
	FORMAT((stderr, "FormatProposal: %s\n", fmt));
#endif
	// everything checks out and we've filled in the
	// wildcards that we care about.
	return B_OK;
err:
	// we didn't like whatever they suggested. Counter-
	// propose with something acceptable and return an
	// error.
	FormatSuggestionRequested(B_MEDIA_RAW_AUDIO, 0, format);
	return B_MEDIA_BAD_FORMAT;
}


status_t SoundProducer::FormatChangeRequested(
	const media_source & source,
	const media_destination & destination,
	media_format * io_format,
	int32 * /* deprecated */)
{
	// first make sure the format is a reasonable one
	// by our standards. If it is, set ourselves to the
	// new format. Two important details here:
	// * We increment the change tag every time we
	//   change the format. This could potentially
	//   allow the downstream consumer to know what
	//   buffer went with which format.
	// * We need to reallocate the correct # of buffers
	//   when our format changes.
	status_t err = FormatProposal(source, io_format);
	if (err < B_OK) return err;
		
	m_output.format = *io_format;
	if (m_notifyHook) {
		(*m_notifyHook)(m_cookie, B_FORMAT_CHANGED, &m_output.format.u.raw_audio);
	}
	else {
		Notify(B_FORMAT_CHANGED, &m_output.format.u.raw_audio);
	}
	alloc_buffers();
	return B_OK;
}


status_t SoundProducer::GetNextOutput(
	int32 * cookie,
	media_output * out_output)
{
	NODE((stderr, "SoundProducer: GetNextInput()\n"));
	//	The "next" is kind of misleading, since it's also used for
	//	getting the first (and only) output.
	if (*cookie == 0) {
		// even if there's no current connection, m_output has been
		// set up to provide all the details about the kinds of
		// connections we like.
		*out_output = m_output;
		*cookie = 1;
	}
	else {
		// There's only one output.
		return B_BAD_INDEX;
	}
	return B_OK;
}


status_t SoundProducer::DisposeOutputCookie(
	int32 /* cookie */)
{
	//	We didn't allocate any memory or set any state in GetNextOutput()
	//	so this function is basically a no-op.
	return B_OK;
}


status_t SoundProducer::SetBufferGroup(
	const media_source & for_source,
	BBufferGroup * group)
{
	if (for_source != m_output.source) {
		// We only accept buffer groups aimed at our single output.
		NODE((stderr, "SoundProducer::SetBufferGroup(): bad source\n"));
		return B_MEDIA_BAD_SOURCE;
	}
	// We always own our buffer group. If we're being told to use a
	// different buffer group, we delete our current buffer group
	// first.
	// 
	// Note: Deleting a buffer group attempts to reclaim all of its
	// outstanding buffers. So, if we're holding on to any buffers,
	// we must recycle them before deleting the group, unless we
	// crave a big hunk o' deadlock pie. 
	if (group != m_buffers) {
		delete m_buffers;
		m_buffers = group;
	}
	return B_OK;
}


status_t SoundProducer::VideoClippingChanged(
	const media_source & /* for_source */,
	int16 /* num_shorts */,
	int16 * /* clip_data */,
	const media_video_display_info & /* display */,
	int32 * /* out_from_change_count */)
{
	// Anybody who attempts to treat us like a video node
	// is sadly mistaken.
	return B_ERROR;
}


status_t SoundProducer::GetLatency(
	bigtime_t * out_latency)
{
	status_t err = BBufferProducer::GetLatency(out_latency);
	if (err >= B_OK) {
		*out_latency = TotalLatency();
	}
	return err;
}

status_t SoundProducer::PrepareToConnect(
	const media_source & what,
	const media_destination & where,
	media_format * format,
	media_source * out_source,
	char * out_name)
{
	NODE((stderr, "SoundProducer::PrepareToConnect()\n"));
	// We only accept connections to our single output, and only
	// when that output isn't already connected to something.
	if (what != m_output.source) {
		NODE((stderr, "SoundProducer::PrepareToConnect(): bad source\n"));
		return B_MEDIA_BAD_SOURCE;
	}
	if (m_output.destination != media_destination::null) {
		NODE((stderr, "SoundProducer::PrepareToConnect(): already connected\n"));
		return B_MEDIA_BAD_DESTINATION;
	}
	
	// Generally, at this stage, the format should already
	// be agreed upon. But, to be safe, we'll do some last
	// sanity checks, filling in wildcards and double-
	// checking to make sure the format is really compatible.
	// This is our last chance to bail on the connection
	// if something isn't right.
	
	// The only thing we'll fill in here is the buffer size,
	// since we've had a chance to contribute our say to
	// every other part of the format.
	if (!m_output.format.u.raw_audio.buffer_size) {
		if (format->u.raw_audio.buffer_size > 0) {
			m_output.format.u.raw_audio.buffer_size = format->u.raw_audio.buffer_size;
		}
		else {
			format->u.raw_audio.buffer_size = m_raw_format.buffer_size;
		}
	}
#if DEBUG
		char fmt[100];
		string_for_format(m_output.format, fmt, 100);
		FORMAT((stderr, "we're suggesting %s\n", fmt));
		string_for_format(*format, fmt, 100);
		FORMAT((stderr, "he's expecting %s\n", fmt));
#endif

	// format_is_compatible does the sanity-checking work
	// for us; for each raw audio field that's specified
	// in both formats, it checks to make sure that the two
	// fields are equal.
	if (!format_is_compatible(*format, m_output.format)) {
		NODE((stderr, "SoundProducer::PrepareToConnect(): bad format\n"));
		return B_MEDIA_BAD_FORMAT;
	}
	
	// Final steps: everything is OK for us to connect, so
	// we'll go ahead and fill in the format now. When we
	// return OK, PrepareToConnect requires us to reserve a
	// source for the connection. The media server will then
	// call BBufferConsumer::Connected to establish the
	// connection on the consumer end and finalize the
	// destination. Finally, the media server will call our
	// Connect function to resolve the connection for once
	// and for all. We need to keep this source reserved
	// (i.e. not connect it to anything else) until we get a
	// call to Connect. We'll do this by tentatively setting
	// the output's destination, though there's no guarantee
	// that this will be the final destination we end up
	// connecting to.
	m_output.destination = where;
	m_output.format = *format;
	*out_source = m_output.source;
	strncpy(out_name, Name(), B_MEDIA_NAME_LENGTH);
	return B_OK;
}


void SoundProducer::Connect(
	status_t error, 
	const media_source & source,
	const media_destination & destination,
	const media_format & format,
	char * io_name)
{
	// The source had better match up by this point in the game!
	ASSERT(source == m_output.source);
	
	// An error indicates that the consumer had to bail out of the
	// connection for some reason (most likely a race condition where
	// its input(s) were already taken by the time we got to it). So,
	// we free up the output that we had previously reserved for this
	// connection in PrepareToConnect.
	if (error < B_OK) {
		NODE((stderr, "SoundProducer::Connect(): we were told about an error\n"));
		m_output.destination = media_destination::null;
		return;
	}
	
	// Otherwise, the connection has been confirmed! We reset the
	// destination (in case the consumer had to change destinations
	// on us) and reset the format too, though that shouldn't strictly
	// be necessary (recall that the format was already pre-cleared
	// through PrepareToConnect).
	
#if !NDEBUG
	char fmt[100];
	string_for_format(format, fmt, 100);
	FORMAT((stderr, "Connect(): format %s\n", fmt));
#endif

	m_output.destination = destination;
	m_output.format = format;

	// Now that the connection has been set, we need to figure out
	// how many buffers we'll be using for this connection.
	alloc_buffers();

	// Also, now that we're connected, we can determine what our
	// downstream latency *really* is.
	BBufferProducer::GetLatency(&m_downstream_latency);

	// Don't forget to tell them who we are.
	strncpy(io_name, Name(), B_MEDIA_NAME_LENGTH);

	//	Tell whomever is interested that there's now a connection.
	if (m_notifyHook) {
		(*m_notifyHook)(m_cookie, B_CONNECTED, m_output.name);
	}
	else {
		Notify(B_CONNECTED, m_output.name);
	}
}


void SoundProducer::Disconnect(
	const media_source & what,
	const media_destination & consumer)
{
	NODE((stderr, "SoundProducer::Disconnect()\n"));
	// We can't disconnect something which isn't us.
	if (what != m_output.source) {
		NODE((stderr, "SoundProducer::Disconnect(): source is incorrect\t"));
		return;
	}
	// We can't disconnect from someone who isn't connected to us.
	if (consumer != m_output.destination) {
		NODE((stderr, "SoundProducer::Disconnect(): destination is incorrect\n"));
		return;
	}
	//	Tell the interested party that it's time to leave.
	if (m_notifyHook) {
		(*m_notifyHook)(m_cookie, B_DISCONNECTED);
	}
	else {
		Notify(B_DISCONNECTED);
	}
	// Mark ourselves as not-connected. We also clean up
	// the buffer group that we were using.
	m_output.destination = media_destination::null;
	m_output.format.u.raw_audio.buffer_size = 0;
	delete m_buffers;
	m_buffers = 0;
}


void SoundProducer::LateNoticeReceived(
	const media_source & /* what */,
	bigtime_t how_much,
	bigtime_t performance_time)
{
	LATE((stderr, "SoundProducer::LateNoticeReceived(%.4f @ %.4f)\n",
		us_to_s(how_much), us_to_s(performance_time)));
	switch (RunMode()) {
	case B_OFFLINE:
		// If we're not running in real time, we should never receive
		// late notices!
		break;
	case B_DECREASE_PRECISION:
		// We can't really do much here. Maybe our hook functions
		// will be able to do something about this?...
		break;
	case B_INCREASE_LATENCY:
		// Adjust our latencies so that we start sending buffers
		// earlier.
		if (how_much > 3000) {
			how_much = 3000;
		}
		m_downstream_latency += how_much;
		m_private_latency += how_much/2;
		break;
	default:
		// As a simple producer, we can't be recording, so
		// we must be in B_DROP_DATA. Just drop a buffer and
		// hope that catches us up.
		m_frames_played += frames_per_buffer(m_output.format.u.raw_audio);
		break;
	}
	//	Tell whoever's interested that we're running behind.
	if (m_notifyHook) {
		(*m_notifyHook)(m_cookie, B_LATE_NOTICE, how_much, performance_time);
	}
	else {
		Notify(B_LATE_NOTICE, how_much, performance_time);
	}
}


void SoundProducer::EnableOutput(
	const media_source & what,
	bool enabled,
	int32 * /* deprecated */)
{
	if (what != m_output.source) {
		NODE((stderr, "SoundProducer::EnableOutput(): bad source\n"));
		return;
	}
	m_muted = !enabled;
	if (m_output.destination != media_destination::null) {
		// It's important to tell the consumer that we will or
		// won't be sending it data, so it's ready for the data
		// or won't get stuck waiting for data that never comes.
		SendDataStatus(m_muted ? B_DATA_NOT_AVAILABLE : B_DATA_AVAILABLE, 
			m_output.destination, TimeSource()->Now());
	}
}





////////////////////////////////////////////////////////////////////////////////
//
//	Service thread methods
//
////////////////////////////////////////////////////////////////////////////////



void
SoundProducer::DoHookChange(
	void * msg)
{
	//	Tell the old guy we're changing the hooks ...
	if (m_notifyHook) {
		(*m_notifyHook)(m_cookie, B_HOOKS_CHANGED);
	}
	else {
		Notify(B_HOOKS_CHANGED);
	}
	//	... and then do it.
	set_hooks_q * ptr = (set_hooks_q *)msg;
	m_playHook = ptr->process;
	m_notifyHook = ptr->notify;
	m_cookie = ptr->cookie;
}


status_t
SoundProducer::ThreadEntry(
	void * obj)
{
	((SoundProducer *)obj)->ServiceThread();
	return B_OK;
}


void SoundProducer::ServiceThread()
{
	// The Big Bad ServiceThread has three different responsibilities,
	// which makes the producer's service thread considerably more
	// complicated than our SoundConsumer's service thread, and the
	// timing is totally different. The responsibilities, in the
	// order we handle them below, are:
	// Step 1. Watch for pending start, stop, and seek requests, and
	//    handle them at the correct time.
	// Step 2. Receive messages aimed at this node and dispatch them
	//    (typically to HandleMessage()).
	// Step 3. Produce and send buffers to our Consumer at the correct
	//    time.
	NODE((stderr, "SoundProducer::ServiceThread() is alive!\n"));

	//	A media kit message will never be bigger than B_MEDIA_MESSAGE_SIZE.
	//	Avoid wasing stack space by dynamically allocating at start.
	char * msg = new char[B_MEDIA_MESSAGE_SIZE];
	array_delete<char> msg_delete(msg);
	int bad = 0;

	// Now that our thread has started, figure out out our real scheduling
	// latency.
	m_private_latency = estimate_max_scheduling_latency(find_thread(0));
	while (true) {
		// buffer_perf is the next performance time at which a buffer
		// needs to be performed.
		//
		// perf_target is the next performance time at which some event
		// has to occur. If we don't have any pending performance events
		// (such as start or stop), our perf_target will be buffer_perf.
		bigtime_t buffer_perf = frames_duration(m_output.format.u.raw_audio, m_frames_played)+m_delta;
		bigtime_t perf_target = buffer_perf;

		bool ts_running = TimeSource()->IsRunning();
		bool connected = (m_output.destination != media_destination::null);
		bigtime_t timeout = 1000000000000LL;
		
		if (ts_running) {
			////////////////////////////////////////////////////////////
			// Step 1: Handle pending performance events

			// An extended note on timing:
			//
			// Performance events (such as Start, Stop, and
			// Seek) are given a performance time. This
			// performance time is the time at which those
			// events should happen at the final output.
			//
			// One common mistake when figuring out timing
			// is to compare the current performance time
			// to the event's performance time, and handle
			// the event when the current performance time
			// equals the event's performance time. But it
			// takes time for the effects of those events
			// to reach the final output. So, if you calculate
			// timing in this way, you'll always be late.
			//
			// Another common mistake is to do the above,
			// but subtract the total latency from the
			// event's performance time to get the correct
			// performance time at which things should happen.
			// This doesn't work because latency is in units
			// of real time, and performance is in units of
			// performance time, which may vary significantly
			// from real time over large intervals. If you
			// calculate timing in this way, your performance
			// will be erratic at best.
			//
			// The correct way to calculate timing is to use
			// the somewhat misnamed BTimeSource::RealTimeFor,
			// and compare that time to the current real time.
			// RealTimeFor does not give you the absolute real
			// time that corresponds to the performance time,
			// but rather tells you the real time at which you
			// need to do things in order for their effects to
			// reach the final output at the specified
			// performance time. You give it your total latency
			// so that it can take that latency into account
			// when it gives you your real time target.
			bigtime_t now_real = BTimeSource::RealTime();
			if (m_stopping) {
				// There's a pending Stop request.
				if (now_real >= TimeSource()->RealTimeFor(m_tpStop, TotalLatency())) {
					// It's time to handle that Stop request.
					TRANSPORT((stderr, "SoundProducer::Stop() takes effect\n"));
					m_running = false;
					m_stopping = false; // we've now stopped
					if (m_seeking) {
						// There's a seek pending, but we'll defer any
						// pending seeks until we next start.
						m_seeking = false;
					}
					else {
						// Set the seek time so that, by default, we
						// restart where we stopped.
						m_tmSeekTo = m_tpStop-m_delta;
						TRANSPORT((stderr, "Setting m_tmSeekTo to %.4f\n", us_to_s(m_tmSeekTo)));
					}
					// Very important! Tell the Consumer that we're no longer
					// sending data to it, so it doesn't sit around waiting for
					// the buffers that never come.
					if (connected) {
						SendDataStatus(B_DATA_NOT_AVAILABLE, m_output.destination, m_tpStop);
					}
					//	Restart the loop because we changed timing info.
					continue;
				}
				else {
					// It's not quite yet time to Stop, but if it's a sooner
					// event than our next perf_target, set it to be the
					// next perf_target.
					if (m_tpStop < perf_target) {
						TRANSPORT((stderr, "m_tpStop perf_target from %.4f to %.4f\n",
							us_to_s(perf_target), us_to_s(m_tpStop)));
						perf_target = m_tpStop;
					}
				}
			}
			if (m_seeking) {
				// There's a pending Seek request.
				if (now_real >= TimeSource()->RealTimeFor(m_tpSeekAt, TotalLatency())) {
					// It's time to handle that Seek request.
					TRANSPORT((stderr, "SoundProducer::Seek() takes effect\n"));
					m_seeking = false;
					// Seek gives us the relationship between media time
					// and performance time. We represent this relationship
					// via m_delta -- see the description in SoundProducer.h.
					//
					// Note: m_frames_played is not addressed here, which I
					// believe is incorrect. Something to look into when
					// we actually use Seek...
					m_delta = m_tpSeekAt-m_tmSeekTo;
					TRANSPORT((stderr, "setting m_delta to %.4f\n", us_to_s(m_delta)));
					//	Restart the loop because we changed timing info.
					continue;
				}
				else {
					// It's not quite yet time to Seek, but if it's a sooner
					// event than our next perf_target, set it to be the
					// next perf_target.
					if (m_tpSeekAt < perf_target) {
						TRANSPORT((stderr, "m_tpSeek perf_target from %.4f to %.4f\n",
							us_to_s(perf_target), us_to_s(m_tpSeekAt)));
						perf_target = m_tpSeekAt;
					}
				}
			}
			if (m_starting) {
				// There's a pending Start request.
				if (now_real >= TimeSource()->RealTimeFor(m_tpStart, TotalLatency())) {
					// It's time to handle that Start request.
					TRANSPORT((stderr, "SoundProducer::Start() takes effect\n"));
					m_running = true;
					// Seek to the correct point in the media before
					// we begin. This offset might have been set by
					// a Seek operation or the last Stop operation.
					// We seek by setting the offset between media time
					// and performance time.
					m_delta = m_tpStart-m_tmSeekTo;
					TRANSPORT((stderr, "setting m_delta to %.4f\n", us_to_s(m_delta)));
					m_frames_played = 0;
					m_starting = false; // we've now started
				
					// Very important! Tell the Consumer that we'll be sending
					// data to it, so it's expecting us.
					if (connected && !m_muted) {
						SendDataStatus(B_DATA_AVAILABLE, m_output.destination, m_tpStart);
					}
					//	Restart the loop because we changed timing info.
					continue;
				}
				else {
					// It's not quite yet time to Start, but if it's a sooner
					// event than our next perf_target, set it to be the
					// next perf_target.
					if (m_tpStart < perf_target) {
						TRANSPORT((stderr, "m_tpStart perf_target from %.4f to %.4f\n",
							us_to_s(perf_target), us_to_s(m_tpStart)));
						perf_target = m_tpStart;
					}
				}
			}
			
			// Finally, calculate the all-important timeout value.
			// The timeout value is the difference in real time
			// between now and the real time at which our thread
			// needs to handle the next event which occurs at
			// perf_target.
			timeout = TimeSource()->RealTimeFor(perf_target, TotalLatency()) -
				BTimeSource::RealTime();
		}
		else if (m_running) {
			// pathological case: we set a strange timeout value that we
			// can recognize later on if necessary. If we're not connected,
			// we'll make it a huge timeout and catch that error later
			// on.
			timeout = connected ? 9999 : 9999999;
			WARNING((stderr, "SoundProducer: m_running but not (connected && ts_running)\n")); 
		} else {
			WARNING((stderr, "SoundProducer: ! ts_running\n"));
		}

		// Adjust the timeout to make sure it's reasonable.
		if (timeout <= 0) {
			// We needed to start handling the next event before now.
			if ((RunMode() != B_OFFLINE) && (timeout < -50000)) {
				// We're way behind in a real-time run mode --
				// just skip forward in the media to catch up!
				m_delta -= timeout-5000;
			}
			
			// Give us some breathing room to check for messages.
			// We don't simply refuse to handle messages when
			// we're behind so that we can remain responsive even
			// when our buffer-producing ability is maxed out.
			timeout = 1000;
		}
	
		////////////////////////////////////////////////////////////
		// Step 2: Check for pending messages.
		
		// Conveniently enough, if there are no pending messages,
		// this call to read_port_etc will force our thread to
		// wait for the length of timeout. Recall, we just set
		// timeout to be the real time until the next performance
		// event or buffer production needs to happen, whichever
		// comes first.
		int32 code = 0;
		status_t err = read_port_etc(m_port, &code, msg, B_MEDIA_MESSAGE_SIZE, B_TIMEOUT, timeout);
		//	If we received a message, err will be the size of the message (including 0).		
		if (err >= 0) {
			bad = 0;
			NODE((stderr, "SoundProducer msg %#010lx\n", code));
			
			// Check for our private stop message.
			if (code == MSG_QUIT_NOW) {	//	quit now
				NODE((stderr, "SoundProducer quitting\n"));
				if (m_notifyHook) {
					(*m_notifyHook)(m_cookie, B_NODE_DIES, 0);
				}
				else {
					Notify(B_NODE_DIES, 0);
				}
				break;
			}
			//	Else check for our private change-hooks message.
			else if (code == MSG_CHANGE_HOOKS) {
				DoHookChange(msg);
				//	Write acknowledge to waiting thread.
				write_port(((set_hooks_q *)msg)->reply, 0, 0, 0);
			}
			//	Else it is hopefully a regular media kit message; go ahead and
			//	dispatch it. (HandleMessage addresses the case that the message
			//	wasn't understood by anybody.)
			else {
				HandleMessage(code, msg, err);
			}
		}
		//	Timing out means that there was no buffer, which is ok.
		//	Other errors, though, are bad.
		else if (err != B_TIMED_OUT) {
			WARNING((stderr, "SoundProducer::ServiceThread(): port says %#010lx (%s)\n", err, strerror(err)));
			bad++;
			//	If we receive three bad reads with no good messages in between,
			//	things are probably not going to improve (like the port disappeared
			//	or something) so we call it a day.
			if (bad > 3) {
				if (m_notifyHook) {
					(*m_notifyHook)(m_cookie, B_NODE_DIES, bad, err, code, msg);
				}
				else {
					Notify(B_NODE_DIES, bad, err, code, msg);
				}
				break;
			}
		}
		else {
			////////////////////////////////////////////////////////////
			// Step 3: Produce and send a buffer

			bad = 0;
			if (timeout > 1000000) {
				// We set a huge timeout when we were running, but the
				// time source isn't running, and we're not connected?
				continue;	// don't actually play
			}
			
			// Only make a buffer if time is running, and if we are
			// running, connected, enabled, and can get a buffer.
			if (ts_running) {
				if (connected && !m_muted && m_running) {
					BBuffer * buffer = m_buffers->RequestBuffer(
						m_output.format.u.raw_audio.buffer_size);
					if (buffer) {
						bigtime_t now = TimeSource()->Now();
						NODE((stderr, "SoundProducer making a buffer at %Ld.\n", now));
						// Whee, we actually get to make a buffer!
						// Fill the buffer's header fields.
						buffer->Header()->start_time = buffer_perf;
						buffer->Header()->size_used = m_output.format.u.raw_audio.buffer_size;
						//	If there is a play hook, let the interested party have at it!
						if (m_playHook) {
							(*m_playHook)(m_cookie, buffer->Header()->start_time, //-m_delta,
								buffer->Data(), buffer->Header()->size_used,
								m_output.format.u.raw_audio);
						}
						else {
							Play(buffer->Header()->start_time-m_delta,
								buffer->Data(), buffer->Header()->size_used,
								m_output.format.u.raw_audio);
						}
						
						// Update our frame counter and send the buffer off!
						// If the send is successful, the last consumer to use
						// the buffer will recycle it for us.
						m_frames_played += frames_per_buffer(m_output.format.u.raw_audio);
						if (SendBuffer(buffer, m_output.destination) < B_OK) {
							// On the other hand, if the send is unsuccessful,
							// we mustn't forget to recycle the buffer ourselves.
							buffer->Recycle();
						}
					}
					else {
						// Something has gone screwy with our buffer group. To
						// avoid spewing lots of debug output, we'll only print
						// a message once every 256 occurrences.
						static int32 warning_cnt = 0;
						if (!(atomic_add(&warning_cnt, 1) & 255)) {
							WARNING((stderr, "SoundProducer: RequestBuffer() failed\n"));
						}
					}
				}
				else {
					// Time is running, but there's no reason for us to
					// actually process a buffer, becuase we won't be
					// sending it -- we're not connected, running, or
					// we've been told to shut up. We'll fake up timing
					// values so that our next timeout is as short as
					// possible.
					bigtime_t now = TimeSource()->Now();

					// Pretend that our next performance is exactly our
					// latency's worth away (i.e. that we need to start
					// processing right away).
					buffer_perf = now+TotalLatency();
					// Set m_frames_played based on this value, so that the
					// top of the next loop will Do the Right Thing.
					m_frames_played = frames_for_duration(m_output.format.u.raw_audio,
						buffer_perf-m_delta);
				}
			}
			else {
				// We can't do anything when time isn't running.
				TRANSPORT((stderr, "time source is not running\n"));
			}
		}
	}
}


void SoundProducer::alloc_buffers()
{
	delete m_buffers;

	// We'll be passing buffers that we create downstream, until they
	// are recycled after the final consumer is done with them. However,
	// buffers are given to us from a limited pool of memory, and once
	// we've sent out all the buffers that fit in the memory pool, we
	// can't get a new buffer until one or more of those buffers
	// have been recycled. So, we need to make sure that we'll have
	// enough space in our buffer group to continue to get buffers
	// while our other buffers are on their way downstream.

	// To begin with, we'll just ask for the number of buffers we'd need
	// to handle enough data to cover our total latency, and add a couple
	// to be on the safe side.
	bigtime_t latency = TotalLatency();
	int count = buffers_for_duration(m_output.format.u.raw_audio, latency) + 2;

	// But if count is too small, we'd like to have three buffers
	// at the very least, to give us some stability.
	if (count < 3) count = 3;

	// We should set a reasonable maximum on the size of our memory
	// pool. We'll restrict it to 128K.
	if (count*m_output.format.u.raw_audio.buffer_size > 128000) {
		count = 128000/m_output.format.u.raw_audio.buffer_size;
		// We do need to make sure there's at least one buffer, though.
		if (count < 1) count = 1;
	}
	NODE((stderr, "Need %d buffers\n", count));
	m_buffers = new BBufferGroup(m_output.format.u.raw_audio.buffer_size, count);
}


bigtime_t
SoundProducer::ProcessingLatency()
{
	//	We're saying it takes us 500 us to process each buffer. If all we do is
	//	copy the data in, it probably takes much less than that, but it doesn't
	//	hurt to be slightly conservative.
	return 500LL;
}


bigtime_t
SoundProducer::TotalLatency()
{
	// Our total latency is the time it takes to process a buffer,
	// plus the scheduling latency in our buffer processing thread,
	// plus the latency downstream from our node.
	//
	// One source of inaccuracy in this measurement is that downstream
	// latency is always given in units of performance time, where our
	// scheduling and processing latencies are presumably given in 
	// real time units. In practice, performance and real time are 
	// close enough for short amounts of time, and our latency has
	// enough 'give' built into it, that this isn't a significant
	// problem.
	return ProcessingLatency() + m_downstream_latency + m_private_latency;
}


void
SoundProducer::Play(
	bigtime_t /* time */,
	const void * /* data */,
	size_t /* size */,
	const media_raw_audio_format & /* format */)
{
	//	If there is no play hook installed, we instead call this function
	//	for received buffers.
}

void
SoundProducer::Notify(
	int32 /* cause */,
	...)
{
	//	If there is no notification hook installed, we instead call this function
	//	for giving notification of various events.
}



