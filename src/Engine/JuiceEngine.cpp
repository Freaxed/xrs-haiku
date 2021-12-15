/*
 * 
 * Copyright 2006-2008, FunkyIdeaSoftware.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Andrea Anzani <andrea.anzani@gmail.com>
 */

#include "JuiceEngine.h"
#include "Log.h"
#include "Song.h"
#include "Track.h"

#include "MeasureManager.h"
#include "Sequence.h"
#include "Pattern.h"
#include "Note.h"
#include <SoundPlayer.h>

#include "PMixer.h"
#include "PBus.h"
#include "ValuableManager.h"

//temp
#include "NotifierLoopClock.h"

static NotifierLoopClock	the_clock; //put as singleton! (why? can't be a private attribute?)



int min(int a, int b) { return a<b ? a:b; }

JuiceEngine*
JuiceEngine::Get()
{
    static	JuiceEngine*	instance = NULL;
	if (instance == NULL) 
			instance = new JuiceEngine("The Classic Juice Engine");
	return instance;
}
	 
JuiceEngine::JuiceEngine(const char* name):Engine(name),
	fCurrentSong(NULL){

	stream_note[0]=new float[ FRAMES_NUM];
	stream_note[1]=new float[ FRAMES_NUM];

	stream_stream[0]=new float[ FRAMES_NUM];
	stream_stream[1]=new float[ FRAMES_NUM];
		
	
	the_clock.AddTickable(this);
	the_clock.ResetAndNotify();
	
	fPeakLeft = fPeakRight = -1.0; //force to update
	
	//Not sure if it's the right location..
	if (!ValuableManager::Get()->RegisterValuableReceiver(VID_TEMPO_BPM, this))
		LogFatal("JuiceEngine can't register to BPM!");
	
}

JuiceEngine::~JuiceEngine(){
}

void	
JuiceEngine::MessageReceived(BMessage* msg) {
	if (msg->what == MSG_VALUABLE_CHANGED)
			ValuableChanged(msg);
	else
			Engine::MessageReceived(msg);
}

void	
JuiceEngine::ValuableChanged(BMessage* msg) {
	ValuableID vID;
	if (msg->FindString(VAL_ID, 0, &vID) == B_OK) {
		if (vID == VID_TEMPO_BPM) {
			int32 bpm;
			if (msg->FindInt32(VAL_DATA_KEY, 0, &bpm) == B_OK)
				SetBPM(bpm);
		}
	}
}

//tick!
void	
JuiceEngine::TickedHigh(uint64 time,int16 beat,int16 tick) 
{
	if ( tick == 0 )
		process_row(beat);
}
			

// -**** NOT USED ******- //
void JuiceEngine::TickedLow(uint64 time,int16 beat,int16 tick){}
				




void
JuiceEngine::ResetSong(Song* song)
{
	IF_LOCK
	
	fCurrentSong = song;
	SendTrackMessage(SystemReset,0);
	ValuableManager::Get()->UpdateValue(VID_TEMPO_BPM, song->getTempo()); //huge bug: it's not syncronous!

	UNLOCK
}

void	
JuiceEngine::SetBPM(int bpm)
{
	//if (fCurrentSong) fCurrentSong->setTempo(bpm);
	int32 note_size=(size_t)2646000/bpm;
	while(note_size % 4 !=0) note_size++;
	fSamplesPerBeat = note_size; //fCurrentSong->getNoteSize();
	fSamplesPerTick = fSamplesPerBeat/(fDefaultResolution*4);

	SendTrackMessage(TempoChange,(float)fSamplesPerBeat);	
}

int		
JuiceEngine::GetBPM() {

	if(fCurrentSong)
		return fCurrentSong->getTempo();
	else
		return 0;	
}

void		
JuiceEngine::SendTrackMessage(SynthMessage msg, float data){
	
	CHECK_LOCK;
	
	if(fCurrentSong)
	for(int y=0;y<fCurrentSong->getNumberTrack();y++)
		fCurrentSong->getTrackAt(y)->Message(msg,data);

}


void	
JuiceEngine::Starting(){

	BufferPosition=0;

	the_clock.Reset();
	the_clock.SendValue(P0, -1);
	the_clock.SendValue(P1, MeasureManager::Get()->_getCurPat());
	the_clock.SendValue(P2, MeasureManager::Get()->_getCurPos());
	
	Acquire("JuiceEngine::Starting");

   	MeasureManager::Get()->SetPosition(0);
   	
   	Release("JuiceEngine::Startig");
		
	SendTrackMessage(SystemStart,0);

}

void	
JuiceEngine::Stopping(){

	BufferPosition=0;
	
	SendTrackMessage(SystemStop,0);

	the_clock.ResetAndNotify(player->Latency());
}


void	
JuiceEngine::SecureProcessBuffer(void * buffer, size_t size)
{

	PMixer::Get()->ResetBuffers();
	
	if(!fCurrentSong)
	{
		memset(buffer,0,size);
		return;
	}
			
	const int32 num_track = fCurrentSong->getNumberTrack();
	const int 	frames = size/FRAMESIZE;
	
	int 		len=frames;
	
	
	/* Punto 1 : Divisione in sotto bufferini */
		
	while(len) {
	
		int buflen=min(len, (int)ceil(fSamplesPerTick-BufferPosition));
		
		BufferPosition	+=	buflen; 
		
		/* Punto 2 : Processiamo il bufferino di ogni traccia.. */
		
		if(buflen>0 && buflen<=frames) 
			
			for(int x=0;x<num_track;x++) 
			{ 
				Track*	track=(Track*)fCurrentSong->getTrackAt(x);
				
				//Scelta del canale di output (PBus):				
				int   line = track->getRouteLine();
				PBus* a_bus = NULL;	
				
				if (line == 0)
					a_bus = PMixer::Get();
				else
					a_bus = PMixer::Get()->BusAt(line - 1);
					
				if(!a_bus) 
					break; // should never happen! TODO:add warnings log.
				
				// PROCESS
				if(track->getProcessorType() == 1)  
				{
					//voice_list = active voice list
					for(int i=0;i<track->voice_list.CountItems();i++)
					{
						a_bus->SetUsed(true);
						
						XRSVoice v=(XRSVoice)track->voice_list.ItemAt(i);
						int32 ret=track->ProcessVoice(v,stream_note, buflen); 
								
						if (ret>0)
							a_bus->MixBuffer(stream_note,ret,frames-len);
						else	
							rem_list.AddItem((void*)v); //voice is dead.								
					}
							
					if(rem_list.CountItems()>0) 
								RemoveVoices(track);
						
				} 
				else //track stream type:
				{
					if(track->HasData()) 
					{
						a_bus->SetUsed(true); 
						memset(stream_stream[0],0,FRAMES_NUM*sizeof(float));
						memset(stream_stream[1],0,FRAMES_NUM*sizeof(float));
						track->Process(stream_stream, buflen, 0);
						a_bus->MixBuffer(stream_stream,buflen,frames-len);
					}																				
				}
						
			}
		
		
		
		if(BufferPosition>=fSamplesPerTick) 
		{			
			BufferPosition-=fSamplesPerTick;
			the_clock.Tick();
		}
		len -= buflen;		
	}
		
	// Mixer Processing (questa parte tutta nel mixer con un bel Melt..)
	
	float*	fbuffer=(float*)buffer;

	PMixer::Get()->Process((PBus*)NULL,frames); //qui il mixer mixa i vari bus insieme (aggiungendo i vst)

	// we need to notify the meters.. iThink@
	// and we should also do it with the right delay time.. (FIX)
	// and to all the busses.. (FIX)
	if (PMixer::Get()->Used())
	{
		float meter_l = PMixer::Get()->GetLastMaxValue(0);
		float meter_r = PMixer::Get()->GetLastMaxValue(1);
		
		if (meter_l != fPeakLeft ||
			meter_r != fPeakRight ) {
				ValuableManager::Get()->UpdateValue("xrs.mixer.main.meter", meter_r, meter_l);
				
				fPeakLeft  = meter_l;
				fPeakRight = meter_r;
		}
	}
	
	float **mixed = PMixer::Get()->Buffer();
	for(int i=0 ;i<frames; i++){
		fbuffer[(0 + i) * 2    ]	= mixed[0][i];
		fbuffer[(0 + i) * 2 + 1] 	= mixed[1][i];
	}
	
}

void
JuiceEngine::process_row(int32 row)
{
	if(!IsPlaying()) return;
		
	int32 	numtracks=fCurrentSong->getNumberTrack();	
	int 	z = 0;
	bool	clear = false; 
	int 	pat = 0;
	
	z = MeasureManager::Get()->ResetStep();
		
	if(z!=EMPTY_PATTERN)
		while(numtracks--)	
		{
			clear=false;	
			Track*	track=(Track*)fCurrentSong->getTrackAt(numtracks);
			
			z=MeasureManager::Get()->ResetStep();
				
			pat=z;
			
			while(z!=-1)
			{
				
				Pattern* pat=track->getPatternAt(z);
				if(!pat) return;		
				
				Note *n=pat->getNoteAt(row);
					
				if(n->getValue() && track->isOn())
				{
					//Cut_it_self
					if(!clear && track->cit) {
						DeleteVoices(track);
						clear=true;
					}
				
					XRSVoice v=track->newVoice(n,z);
					if(v!=NULL)	AddVoice(track ,v);
				}		
				z=MeasureManager::Get()->SubStep();
			} //end multi pattern!
		}  //end multi-track;
		
	
	bigtime_t  late=player->Latency();
	the_clock.SendValue(P0,row,late);
	
	row++;
	
	if(row > fCurrentSong->getNumberNotes()-1) { 
		row=0;
		MeasureManager::Get()->MoveNextCol();
		the_clock.SendValue(P1,MeasureManager::Get()->_getCurPat(),late);
		the_clock.SendValue(P2,MeasureManager::Get()->_getCurPos(),late);
	}
}


// Voice management

//add a voice to the track list of current voices
void			
JuiceEngine::AddVoice(Track* track ,XRSVoice v) { track->voice_list.AddItem((void*)v); }


// remove the voice from a track list and from the rem_list.
void
JuiceEngine::RemoveVoices(Track* track){
	
	for(int i=0;i<rem_list.CountItems();i++){
		track->voice_list.RemoveItem(rem_list.ItemAt(i));
		track->killVoice(rem_list.ItemAt(i));
	}
	rem_list.MakeEmpty();
}


//delete all the voice in the rem_voice
void
JuiceEngine::DeleteVoices(Track* track){

	for(int i=0;i<track->voice_list.CountItems();i++){
		track->killVoice(track->voice_list.ItemAt(i));
	}
	track->voice_list.MakeEmpty();
}

//XRSVoice JuiceEngine::GetVoice(Track* track,int i){ return (XRSVoice)track->voice_list.ItemAt(i);}


//--


