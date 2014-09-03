// Copyright (c) 2003-2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabSound.h"
#include "AudioContext.h"

namespace WebCore
{
    class Document { public: };
}

namespace LabSound {

    PassRefPtr<WebCore::AudioContext> init() {
        // Initialize threads for the WTF library
        WTF::initializeThreading();
        WTF::initializeMainThread();
        
        // Create an audio context object
        WebCore::Document d;
        WebCore::ExceptionCode ec;
        PassRefPtr<WebCore::AudioContext> ret = WebCore::AudioContext::create(&d, ec);
        ret->lazyInitialize();
        return ret;
    }

    bool connect(WebCore::AudioNode* thisOutput, WebCore::AudioNode* toThisInput) {
        WebCore::ExceptionCode ec = -1;
        thisOutput->connect(toThisInput, 0, 0, ec);
        return ec == -1;
    }

    bool disconnect(WebCore::AudioNode* thisOutput) {
        WebCore::ExceptionCode ec = -1;
        thisOutput->disconnect(0, ec);
        return ec == -1;
    }

	static double MIDIToFrequency(int MIDINote) {
		return 8.1757989156 * pow(2, MIDINote / 12);
	}

	static float truncate(float d, unsigned int numberOfDecimalsToKeep) {
		return roundf(d * powf(10, numberOfDecimalsToKeep)) / powf(10, numberOfDecimalsToKeep);
	}

} // LabSound

