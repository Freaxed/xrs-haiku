#include "MixerEngine.h"
#include "Logger.h"
#include "ETrack.h"
#include "Playlist.h"
#include "PianoRollContainer.h"
#include "SequencerContainer.h"
#include "PlayListDestination.h"

#include "PMixer.h"
extern PMixer	the_mixer;

#include "MixerDestination.h"

MixerDestination	*mixer_destination;
CEvent 				genericEv;			

MixerEngine::MixerEngine(const char* name):TickedEngine(name){

	fCurrentPart = NULL;

	genericEv.SetStart(0);
	genericEv.SetDuration(100);
	genericEv.SetVChannel(0);
	
	service[0] = new float[1024];
	service[1] = new float[1024];			

	mixer_destination = new MixerDestination();
}

MixerEngine::~MixerEngine(){
	delete[] service[0];
	delete[] service[1];
	mixer_destination->FreeAllNotes();
	delete mixer_destination;
}


void	
MixerEngine::SetMediaPart(MediaPart* part){

		if(IsPlaying())
			debugger("The Engine should be stopped before SetPlayingMode!");
		
		
			
		//QUI
		/*
		
			1) Blocco in lettura scrittura (assoluto)
			2) Blocco del thread.
			3) Clear dei varie variabili
			4) set della media_part e playing mode
			5) Rilascio del thread (isPlaying == false)
			6) Notifica stato a altri observer.
			7) sblocco della scrittura/lettura.
			
		*/

		//1)
		StSubjectLock lock(*this);		
		
		//2)
		Acquire("MixerEngine::SetPlayingMode");
		ReallyStop();
		
		//3)
			
		fBeatpos = 0;
		
		//4)
		fCurrentPart = part;
		
		//6) ??
		// STOP ALL?
		// Bisogna anche settare il clock dell'engine
		// con il clock che la media part usa, questo clock
		// tiene conto dei loop-points ed eventualmente di extra-speed
		// e magari anche della pausa? oppure la pausa è dell'engine?
		
		// QUI MANCA QUALCOSA! (MA COSA?)
		
		//5)
		ReallyStart();
		Release("MixerEngine::SetPlayingMode");
		
		//7)
		// sblocco automatico (quando distrutto il lock)
}

void	
MixerEngine::Starting(){
		
		fClock->Reset();
		mixer_destination->FreeAllNotes();
		
		BMessage update(UPDATED);
		update.AddInt16("operation",PLAYING_PART);
		PostUpdate(&update);
}

void	
MixerEngine::Stopping(){

		mixer_destination->FreeAllNotes();
		
		BMessage update(UPDATED);
		update.AddInt16("operation",STOPPED_PART);
		PostUpdate(&update);
}

	
void	
MixerEngine::TickedHigh(uint64 time){

	if(fCurrentPart){
			// FIX per evitare questo (il parametro del &ev)
			// occorre ampliare il concetto di Clock
			// OGNI MediaPart ha un ref ad un clock che viene installato
			// dall'engine al SetMediaPart.
			// è quello che stabilisce da dove inizia e dove finisce il blocco.
			genericEv.SetStart(0);
			genericEv.SetDuration(fCurrentPart->BeatLength() * fClock->Resolution());
			genericEv.note.destination = mixer_destination;
			fCurrentPart->AnalyzeEvents(time,&genericEv);
	}
}

void	
MixerEngine::TickedLow(uint64 time){
		
	if(fCurrentPart){
		genericEv.SetStart(time);
		genericEv.SetDuration(0);
		genericEv.xnote.part = fCurrentPart;
		//genericEv.note.destination = mixer_destination;
		LogInfo("TickedLow");
		
		mixer_destination->Execute(Track::STOP_ALL_NOTES,&genericEv,0);
		
		genericEv.SetStart(time);
		genericEv.SetDuration(0);
		genericEv.xnote.part = fCurrentPart;
		//genericEv.note.destination = mixer_destination;
		
		mixer_destination->Execute(Track::STOP_ALL_SOUNDS,&genericEv,0);
	}
}


void	
MixerEngine::FlushEvents(float* buffer,size_t frames,size_t spiaz){

	/* soluzione temporanea (ma funzionante) */
	/*
		l'idea ottimizzata (style 1.4)
		-ogni traccia fa il process su stream MA con lo spiaz attivo.
		-se necessario a fine ciclo di mix (FlushAll)
		 per ogni bus usato si fa il Process (okkio alla tail dei VST)
		-sempre nello stesso punto si chiama the_mixer.Process e poi
		 si transform lo stream in buffer.
	*/
	
	memset(service[0],0x00,sizeof(float)*1024);
	memset(service[1],0x00,sizeof(float)*1024);
	
	if(fCurrentPart) //DO PROCESS!
		fCurrentPart->FlushEvents(service,frames,spiaz);
}	

void	
MixerEngine::FlushAll(float* buffer,size_t frames){
	
	if(!fCurrentPart) return;
	// here we should ask the mixer for the mixer magic soup!
	
	the_mixer.Process((PBus*)NULL,frames);
	
	
	float **mixed = the_mixer.Buffer();
	for(uint i=0 ;i<frames; i++){
		buffer[(0 + i) * 2    ]	= mixed[0][i];
		buffer[(0 + i) * 2 + 1] = mixed[1][i];
	}
	the_mixer.ResetBuffers();
}

void
MixerEngine::StopCurrentPart(){
	Stop();
	TickedLow(0);
	fBeatpos = 0;
}

