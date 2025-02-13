#include <napi.h>
#include <Trill.h>
#include <memory>
#include <string>

class TrillWrapper : public Napi::ObjectWrap<TrillWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "Trill", {
            InstanceMethod("readI2C", &TrillWrapper::ReadI2C),
            InstanceMethod("getNumChannels", &TrillWrapper::GetNumChannels),
            InstanceMethod("getRawData", &TrillWrapper::GetRawData),
            InstanceMethod("printDetails", &TrillWrapper::PrintDetails)
        });

        Napi::FunctionReference* constructor = new Napi::FunctionReference();
        *constructor = Napi::Persistent(func);
        env.SetInstanceData(constructor);

        exports.Set("Trill", func);
        return exports;
    }

    TrillWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TrillWrapper>(info) {
        Napi::Env env = info.Env();

        // Hardcoded values: bus 1, CRAFT device
        const int i2c_bus = 1;
        printf("Attempting to setup Trill CRAFT device on bus %d\n", i2c_bus);
        
        trill = std::make_unique<Trill>();
        int result = trill->setup(i2c_bus, Trill::CRAFT);
        
        if (result != 0) {
            char errorMsg[256];
            snprintf(errorMsg, sizeof(errorMsg), 
                    "Failed to initialize Trill device. Error code: %d, errno: %d (%s)", 
                    result, errno, strerror(errno));
            Napi::Error::New(env, errorMsg).ThrowAsJavaScriptException();
            return;
        }
        
        // Set the device to DIFF mode
        trill->setMode(Trill::DIFF);
        printf("Trill device successfully initialized in RAW mode\n");
    }

private:
    std::unique_ptr<Trill> trill;

    Napi::Value ReadI2C(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        int result = trill->readI2C();
        return Napi::Number::New(env, result);
    }

    Napi::Value GetNumChannels(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        return Napi::Number::New(env, trill->getNumChannels());
    }

    Napi::Value GetRawData(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        
        Napi::Array result = Napi::Array::New(env);
        for (size_t i = 0; i < trill->getNumChannels(); i++) {
            result.Set(i, Napi::Number::New(env, trill->rawData[i]));
        }
        return result;
    }

    Napi::Value PrintDetails(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        trill->printDetails();
        return env.Undefined();
    }
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    return TrillWrapper::Init(env, exports);
}

NODE_API_MODULE(trill, Init)
