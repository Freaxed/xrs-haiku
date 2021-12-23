#ifndef VST_ENUMS_H
#define VST_ENUMS_H

enum
{
	effOpen = 0,		// initialise
	effClose,			// exit, release all memory and other resources!

	effSetProgram,		// program no in <value>
	effGetProgram,		// return current program no.
	effSetProgramName,	// user changed program name (max 24 char + 0) to as passed in string 
	effGetProgramName,	// stuff program name (max 24 char + 0) into string 

	effGetParamLabel,	// stuff parameter <index> label (max 8 char + 0) into string
						// (examples: sec, dB, type)
	effGetParamDisplay,	// stuff parameter <index> textual representation into string
						// (examples: 0.5, -3, PLATE)
	effGetParamName,	// stuff parameter <index> label (max 8 char + 0) into string
						// (examples: Time, Gain, RoomType) 
	effGetVu,			// called if (flags & (effFlagsHasClip | effFlagsHasVu))

	// system
	effSetSampleRate,	// in opt (float value in Hz; for example 44100.0Hz)
	effSetBlockSize,	// in value (this is the maximun size of an audio block,
						// pls check sampleframes in process call)
	effMainsChanged,	// the user has switched the 'power on' button to
						// value (0 off, else on). This only switches audio
						// processing; you should flush delay buffers etc.
	
	// editor
	effEditGetRect,		// stuff rect (top, left, bottom, right) into ptr
	effEditOpen,		// system dependant Window pointer in ptr
	effEditClose,		// no arguments
	effEditDraw,		// draw method, ptr points to rect (MAC Only)
	effEditMouse,		// index: x, value: y (MAC Only)
	effEditKey,			// system keycode in value
	effEditIdle,		// no arguments. Be gentle!
	effEditTop,			// window has topped, no arguments
	effEditSleep,		// window goes to background
	
	effIdentify,		// returns 'NvEf'
	effGetChunk,		// host requests pointer to chunk into (void**)ptr, byteSize returned
	effSetChunk,		// plug-in receives saved chunk, byteSize passed
 
	effNumOpcodes		
};


#endif // _H
