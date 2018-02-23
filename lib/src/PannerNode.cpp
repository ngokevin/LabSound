#include <PannerNode.h>

namespace webaudio {

enum ModelType {
  ModelLinear = 0,
  ModelInverse = 1,
  ModelExponential = 2,
};

PannerNode::PannerNode() {}

PannerNode::~PannerNode() {}

Handle<Object> PannerNode::Initialize(Isolate *isolate, Local<Value> fakeAudioParamCons) {
  Nan::EscapableHandleScope scope;

  // constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(JS_STR("PannerNode"));

  // prototype
  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  AudioNode::InitializePrototype(proto);
  PannerNode::InitializePrototype(proto);

  Local<Function> ctorFn = ctor->GetFunction();

  Nan::SetMethod(ctorFn, "setPath", SetPath);
  ctorFn->Set(JS_STR("FakeAudioParam"), fakeAudioParamCons);

  return scope.Escape(ctorFn);
}

void PannerNode::InitializePrototype(Local<ObjectTemplate> proto) {
  Nan::SetAccessor(proto, JS_STR("coneInnerAngle"), ConeInnerAngleGetter, ConeInnerAngleSetter);
  Nan::SetAccessor(proto, JS_STR("coneOuterAngle"), ConeOuterAngleGetter, ConeOuterAngleSetter);
  Nan::SetAccessor(proto, JS_STR("distanceModel"), DistanceModelGetter, DistanceModelSetter);
  Nan::SetAccessor(proto, JS_STR("maxDistance"), MaxDistanceGetter, MaxDistanceSetter);
  Nan::SetAccessor(proto, JS_STR("panningModel"), PanningModelGetter, PanningModelSetter);
  Nan::SetAccessor(proto, JS_STR("refDistance"), RefDistanceGetter, RefDistanceSetter);
  Nan::SetAccessor(proto, JS_STR("rolloffFactor"), RolloffFactorGetter, RolloffFactorSetter);
  Nan::SetMethod(proto, "setPosition", SetPosition);
  Nan::SetMethod(proto, "setOrientation", SetOrientation);
}

NAN_METHOD(PannerNode::New) {
  Nan::HandleScope scope;

  if (info[0]->IsObject() && info[0]->ToObject()->Get(JS_STR("constructor"))->ToObject()->Get(JS_STR("name"))->StrictEquals(JS_STR("AudioContext"))) {
    Local<Object> audioContextObj = Local<Object>::Cast(info[0]);

    PannerNode *pannerNode = new PannerNode();
    Local<Object> pannerNodeObj = info.This();
    pannerNode->Wrap(pannerNodeObj);

    shared_ptr<lab::PannerNode> labPannerNode = make_shared<lab::PannerNode>(defaultAudioContext->sampleRate(), PannerNode::path);

    pannerNode->context.Reset(audioContextObj);
    pannerNode->audioNode = labPannerNode;

    Local<Function> fakeAudioParamConstructor = Local<Function>::Cast(pannerNodeObj->Get(JS_STR("constructor"))->ToObject()->Get(JS_STR("FakeAudioParam")));

    Local<Object> positionXAudioParamObj = fakeAudioParamConstructor->NewInstance(0, nullptr);
    FakeAudioParam *positionXAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(positionXAudioParamObj);
    positionXAudioParam->getter = [&]() { return labPannerNode->position().x; };
    pannerNodeObj->Set(JS_STR("positionX"), positionXAudioParamObj);

    Local<Object> positionYAudioParamObj = fakeAudioParamConstructor->NewInstance(0, nullptr);
    FakeAudioParam *positionYAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(positionYAudioParamObj);
    positionYAudioParam->getter = [&]() { return labPannerNode->position().y; };
    pannerNodeObj->Set(JS_STR("positionY"), positionYAudioParamObj);

    Local<Object> positionZAudioParamObj = fakeAudioParamConstructor->NewInstance(0, nullptr);
    FakeAudioParam *positionZAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(positionZAudioParamObj);
    positionZAudioParam->getter = [&]() { return labPannerNode->position().z; };
    pannerNodeObj->Set(JS_STR("positionZ"), positionZAudioParamObj);

    Local<Object> orientationXAudioParamObj = fakeAudioParamConstructor->NewInstance(0, nullptr);
    FakeAudioParam *orientationXAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(orientationXAudioParamObj);
    orientationXAudioParam->getter = [&]() { return labPannerNode->orientation().x; };
    pannerNodeObj->Set(JS_STR("orientationX"), orientationXAudioParamObj);

    Local<Object> orientationYAudioParamObj = fakeAudioParamConstructor->NewInstance(0, nullptr);
    FakeAudioParam *orientationYAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(orientationYAudioParamObj);
    orientationYAudioParam->getter = [&]() { return labPannerNode->orientation().y; };
    pannerNodeObj->Set(JS_STR("orientationY"), orientationYAudioParamObj);

    Local<Object> orientationZAudioParamObj = fakeAudioParamConstructor->NewInstance(0, nullptr);
    FakeAudioParam *orientationZAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(orientationZAudioParamObj);
    orientationZAudioParam->getter = [&]() { return labPannerNode->orientation().z; };
    pannerNodeObj->Set(JS_STR("orientationZ"), orientationZAudioParamObj);

    info.GetReturnValue().Set(pannerNodeObj);
  } else {
    Nan::ThrowError("invalid arguments");
  }
}

NAN_GETTER(PannerNode::ConeInnerAngleGetter) {
  Nan::HandleScope scope;

  PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
  shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

  float coneInnerAngle = labPannerNode->coneInnerAngle();

  info.GetReturnValue().Set(JS_NUM(coneInnerAngle));
}

NAN_SETTER(PannerNode::ConeInnerAngleSetter) {
  Nan::HandleScope scope;

  if (value->IsNumber()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    float newValue = value->NumberValue();
    labPannerNode->setConeInnerAngle(newValue);
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_GETTER(PannerNode::ConeOuterAngleGetter) {
  Nan::HandleScope scope;

  PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
  shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

  float coneOuterAngle = labPannerNode->coneOuterAngle();

  info.GetReturnValue().Set(JS_NUM(coneOuterAngle));
}

NAN_SETTER(PannerNode::ConeOuterAngleSetter) {
  Nan::HandleScope scope;

  if (value->IsNumber()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    float newValue = value->NumberValue();
    labPannerNode->setConeOuterAngle(newValue);
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_GETTER(PannerNode::DistanceModelGetter) {
  Nan::HandleScope scope;

  PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
  shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

  Local<String> result;
  ModelType distanceModel = (ModelType)labPannerNode->distanceModel();
  switch (distanceModel) {
    case ModelType::ModelLinear: {
      result = Nan::New<String>("linear").ToLocalChecked();
      break;
    }
    case ModelType::ModelInverse: {
      result = Nan::New<String>("inverse").ToLocalChecked();
      break;
    }
    case ModelType::ModelExponential: {
      result = Nan::New<String>("exponential").ToLocalChecked();
      break;
    }
    default: {
      result = Nan::New<String>("").ToLocalChecked();
      break;
    }
  }

  info.GetReturnValue().Set(result);
}

NAN_SETTER(PannerNode::DistanceModelSetter) {
  Nan::HandleScope scope;

  if (value->IsString()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    v8::String::Utf8Value valueUtf8(value->ToString());
    string valueString(*valueUtf8, valueUtf8.length());

    ModelType distanceModel;
    if (valueString == "linear") {
      distanceModel = ModelType::ModelLinear;
    } else if (valueString == "inverse") {
      distanceModel = ModelType::ModelInverse;
    } else if (valueString == "exponential") {
      distanceModel = ModelType::ModelExponential;
    } else {
      distanceModel = ModelType::ModelLinear;
    }

    labPannerNode->setDistanceModel(distanceModel);
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_GETTER(PannerNode::MaxDistanceGetter) {
  Nan::HandleScope scope;

  PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
  shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

  float maxDistance = labPannerNode->maxDistance();

  info.GetReturnValue().Set(JS_NUM(maxDistance));
}

NAN_SETTER(PannerNode::MaxDistanceSetter) {
  Nan::HandleScope scope;

  if (value->IsNumber()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    float newValue = value->NumberValue();
    labPannerNode->setMaxDistance(newValue);
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_GETTER(PannerNode::PanningModelGetter) {
  Nan::HandleScope scope;

  PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
  shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

  Local<String> result;
  lab::PanningMode panningModel = labPannerNode->panningModel();
  switch (panningModel) {
    case lab::PanningMode::EQUALPOWER: {
      result = Nan::New<String>("equalpower").ToLocalChecked();
      break;
    }
    case lab::PanningMode::HRTF: {
      result = Nan::New<String>("HRTF").ToLocalChecked();
      break;
    }
    default: {
      result = Nan::New<String>("").ToLocalChecked();
      break;
    }
  }

  info.GetReturnValue().Set(result);
}

NAN_SETTER(PannerNode::PanningModelSetter) {
  Nan::HandleScope scope;

  if (value->IsString()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    v8::String::Utf8Value valueUtf8(value->ToString());
    string valueString(*valueUtf8, valueUtf8.length());

    lab::PanningMode panningModel;
    if (valueString == "equalpower") {
      panningModel = lab::PanningMode::EQUALPOWER;
    } else if (valueString == "HRTF") {
      panningModel = lab::PanningMode::HRTF;
    } else {
      panningModel = lab::PanningMode::EQUALPOWER;
    }

    labPannerNode->setPanningModel(panningModel);
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_GETTER(PannerNode::RefDistanceGetter) {
  Nan::HandleScope scope;

  PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
  shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

  float refDistance = labPannerNode->refDistance();

  info.GetReturnValue().Set(JS_NUM(refDistance));
}

NAN_SETTER(PannerNode::RefDistanceSetter) {
  Nan::HandleScope scope;

  if (value->IsNumber()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    float newValue = value->NumberValue();
    labPannerNode->setRefDistance(newValue);
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_GETTER(PannerNode::RolloffFactorGetter) {
  Nan::HandleScope scope;

  PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
  shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

  float rolloffFactor = labPannerNode->rolloffFactor();

  info.GetReturnValue().Set(JS_NUM(rolloffFactor));
}

NAN_SETTER(PannerNode::RolloffFactorSetter) {
  Nan::HandleScope scope;

  if (value->IsNumber()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    float newValue = value->NumberValue();
    labPannerNode->setRolloffFactor(newValue);
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_METHOD(PannerNode::SetPosition) {
  Nan::HandleScope scope;

  if (info[0]->IsNumber() && info[1]->IsNumber() && info[2]->IsNumber()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    float x = info[0]->NumberValue();
    float y = info[1]->NumberValue();
    float z = info[2]->NumberValue();

    labPannerNode->setPosition(x, y, z);
  } else {
    Nan::ThrowError("AnalyserNode::GetFloatFrequencyData: invalid arguments");
  }
}

NAN_METHOD(PannerNode::SetOrientation) {
  Nan::HandleScope scope;

  if (info[0]->IsNumber() && info[1]->IsNumber() && info[2]->IsNumber()) {
    PannerNode *pannerNode = ObjectWrap::Unwrap<PannerNode>(info.This());
    shared_ptr<lab::PannerNode> labPannerNode = *(shared_ptr<lab::PannerNode> *)(&pannerNode->audioNode);

    float x = info[0]->NumberValue();
    float y = info[1]->NumberValue();
    float z = info[2]->NumberValue();

    labPannerNode->setOrientation(x, y, z);
  } else {
    Nan::ThrowError("AnalyserNode::GetFloatFrequencyData: invalid arguments");
  }
}

NAN_METHOD(PannerNode::SetPath) {
  Nan::HandleScope scope;

  if (info[0]->IsString()) {
    v8::String::Utf8Value pathValue(info[0]->ToString());

    PannerNode::path = *pathValue;
  } else {
    Nan::ThrowError("PannerNode::SetPath: invalid arguments");
  }
}

string PannerNode::path;

}