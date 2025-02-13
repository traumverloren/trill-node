#include <napi.h>
#include "I2c.h"
#include "Trill.h"
#include <memory>

class TrillWrapper : public Napi::ObjectWrap<TrillWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        Napi::Function func = DefineClass(env, "TrillWrapper", {
            InstanceMethod("setup", &TrillWrapper::Setup),
            InstanceMethod("readI2C", &TrillWrapper::ReadI2C),
            InstanceMethod("getNumTouches", &TrillWrapper::GetNumTouches),
            InstanceMethod("getTouchLocation", &TrillWrapper::GetTouchLocation),
            InstanceMethod("getTouchHorizontalLocation", &TrillWrapper::GetTouchHorizontalLocation),
            InstanceMethod("is2D", &TrillWrapper::Is2D),
            InstanceMethod("getMode", &TrillWrapper::GetMode),
            InstanceMethod("getRawData", &TrillWrapper::GetRawData),
            InstanceMethod("getNumChannels", &TrillWrapper::GetNumChannels)
        });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set("TrillWrapper", func);
        return exports;
    }

    TrillWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TrillWrapper>(info) {
        trill = std::make_unique<Trill>();
    }

private:
    static Napi::FunctionReference constructor;
    std::unique_ptr<Trill> trill;

    Napi::Value Setup(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        if (info.Length() < 2) {
            Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
            return env.Null();
        }

        int i2cBus = info[0].As<Napi::Number>().Int32Value();
        std::string deviceName = info[1].As<Napi::String>().Utf8Value();
        
        // Optional address parameter
        uint8_t address = 255;  // default value
        if (info.Length() > 2) {
            address = info[2].As<Napi::Number>().Uint32Value();
        }

        Trill::Device device = Trill::getDeviceFromName(deviceName);
        if (device == Trill::UNKNOWN) {
            Napi::Error::New(env, "Invalid device name").ThrowAsJavaScriptException();
            return env.Null();
        }

        int result = trill->setup(i2cBus, device, address);
        return Napi::Number::New(env, result);
    }

    Napi::Value ReadI2C(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        trill->readI2C();
        return env.Undefined();
    }

    Napi::Value GetNumTouches(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        return Napi::Number::New(env, trill->getNumTouches());
    }

    Napi::Value GetTouchLocation(const Napi::Call
