#ifndef _AUDIO_PARAM_H_
#define _AUDIO_PARAM_H_

#include <v8.h>
#include <node.h>
#include <nan.h>
#include <string>
#include "LabSound/extended/LabSound.h"
#include <defines.h>
#include <globals.h>

using namespace std;
using namespace v8;
using namespace node;

namespace webaudio {

class AudioParam : public ObjectWrap {
public:
  static Handle<Object> Initialize(Isolate *isolate);

protected:
  static NAN_METHOD(New);
  static NAN_METHOD(Connect);
  static NAN_GETTER(DefaultValueGetter);
  static NAN_GETTER(MaxValueGetter);
  static NAN_GETTER(MinValueGetter);
  static NAN_GETTER(ValueGetter);
  static NAN_SETTER(ValueSetter);

  AudioParam();
  ~AudioParam();

protected:
  shared_ptr<lab::AudioParam> audioParam;

  friend class GainNode;
  friend class StereoPannerNode;
};

}

#endif