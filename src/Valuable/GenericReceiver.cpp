/*
 * Copyright by Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GenericReceiver.h"

void GenericReceiver::MessageReceived(BMessage* msg) {
		switch(msg->what) {
            case MSG_VALUABLE_CHANGED:
            {
                int32   volume;
                int32   bpm;
                float	vmeter_l, vmeter_r;
                if (ValuableTools::SearchValues("xrs.mixer.main.volume", msg, &volume)){  
                	LogInfo("GenericReceiver: xrs.mixer.main.volume set to %d", volume);
                } else
                if (ValuableTools::SearchValues("xrs.time.bpm", msg, &bpm)){
                    LogInfo("GenericReceiver: xrs.time.bpm set to %d", bpm);
                } else
                if (ValuableTools::SearchValues<float>("xrs.mixer.main.vumeter", msg, &vmeter_l, &vmeter_r))
                {
                	LogInfo("GenericReceiver: xrs.mixer.main.vumeter set  %f | %f", vmeter_l, vmeter_r);
                }
            }
            break;
			default:
			break;
		}
		BLooper::MessageReceived(msg);
	}
