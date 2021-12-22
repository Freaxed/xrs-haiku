/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CommonValuableID_H
#define CommonValuableID_H


#define VID_TEMPO_BPM  			"xrs.time.bpm"
#define VID_TEMPO_BEAT 			"xrs.time.beat"
#define VID_TEMPO_PATTERN 		"xrs.time.singlepattern" //the number of pattern is playing (when ALL in  OFF
#define VID_TEMPO_MEASURE		"xrs.time.measure" //the number of measure playing (when ALL is ON)


// MIXER VIDs

// "xrs.mixer.line.0.volume" is the main gain (0 line in the main output)

#define	MIXERLINES_COUNT		5 //0 main + 1..4 subline

#define	BASE_VID_MIXER_LINE		ValuableID("xrs.mixer.line.")
#define BASE_VID_VOLUME			".volume"
#define BASE_VID_PAN			".pan"
#define BASE_VID_METER			".meter"

#define VID_MIXER_LIN_VOL(X)		BASE_VID_MIXER_LINE.Append((char)('0' + (uint8)X), 1).Append(BASE_VID_VOLUME)
#define VID_MIXER_LIN_PAN(X)		BASE_VID_MIXER_LINE.Append((char)('0' + (uint8)X), 1).Append(BASE_VID_PAN)
#define VID_MIXER_LIN_MET(X)		BASE_VID_MIXER_LINE.Append((char)('0' + (uint8)X), 1).Append(BASE_VID_METER)

#define VID_MIXER_MAIN_VOL			VID_MIXER_LIN_VOL(0)
#define VID_MIXER_MAIN_PAN			VID_MIXER_LIN_PAN(0)
#define VID_MIXER_MAIN_MET			VID_MIXER_LIN_MET(0)





#endif // CommonValuableID_H
