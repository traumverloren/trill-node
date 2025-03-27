#include <napi.h>
#include <Trill.h>
#include <memory>
#include <string>

// This class wraps the C++ Trill class to make it available in Node.js
class TrillWrapper : public Napi::ObjectWrap<TrillWrapper> {
public:
    // Define the JavaScript class with these methods
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "Trill", {
            // Subset of needed methods from Trill class
            InstanceMethod("readI2C", &TrillWrapper::ReadI2C),
            InstanceMethod("getNumChannels", &TrillWrapper::GetNumChannels),
            InstanceMethod("getRawData", &TrillWrapper::GetRawData),
            InstanceMethod("printDetails", &TrillWrapper::PrintDetails),
            InstanceMethod("setNoiseThreshold", &TrillWrapper::SetNoiseThreshold),
            InstanceMethod("setPrescaler", &TrillWrapper::SetPrescaler),
	        InstanceMethod("updateBaseline", &TrillWrapper::UpdateBaseline)
        });


        // Sets up the constructor for creating new instances
        Napi::FunctionReference* constructor = new Napi::FunctionReference();
        *constructor = Napi::Persistent(func);
        env.SetInstanceData(constructor);

        // Makes the class available as exports.Trill
        exports.Set("Trill", func);
        return exports;
    }

    // This constructor runs when 'new Trill()' is called in JavaScript
    TrillWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TrillWrapper>(info) {
        Napi::Env env = info.Env();

        // Check if address parameter is provided
        if (info.Length() < 1) {
            Napi::TypeError::New(env, "Address parameter is required")
                .ThrowAsJavaScriptException();
            return;
        }

        // Get the address parameter & convert to uint8_t
        uint8_t address = info[0].As<Napi::Number>().Uint32Value();
        const int i2c_bus = 1;  // Hardcoded bus value
        
        printf("Attempting to setup Trill CRAFT device on bus %d at address 0x%02X\n", i2c_bus, address);
        
        trill = std::make_unique<Trill>();
        int result = trill->setup(i2c_bus, Trill::CRAFT, address);
        
        if (result != 0) {
            char errorMsg[256];
            snprintf(errorMsg, sizeof(errorMsg), 
                    "Failed to initialize Trill device at address 0x%02X. Error code: %d, errno: %d (%s)", 
                    address, result, errno, strerror(errno));
            Napi::Error::New(env, errorMsg).ThrowAsJavaScriptException();
            return;
        }
        
        // Set the device to DIFF mode
        result = trill->setMode(Trill::DIFF);
        if (result != 0) {
            char errorMsg[256];
            snprintf(errorMsg, sizeof(errorMsg), 
                    "Failed to set DIFF mode for device at address 0x%02X. Error code: %d", 
                    address, result);
            Napi::Error::New(env, errorMsg).ThrowAsJavaScriptException();
            return;
        }

        printf("Trill device at address 0x%02X successfully initialized in DIFF mode\n", address);
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

    Napi::Value SetNoiseThreshold(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Check if threshold parameter is provided
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Threshold parameter is required")
            .ThrowAsJavaScriptException();
        return env.Undefined();
    }

        // Get the threshold parameter & convert to float
        float threshold = info[0].As<Napi::Number>().FloatValue();
        
        // Call the C++ method
        int result = trill->setNoiseThreshold(threshold);
        
        // Return the result
        return Napi::Number::New(env, result);
    }

   Napi::Value SetPrescaler(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Check if prescaler parameter is provided
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Prescaler parameter is required")
            .ThrowAsJavaScriptException();
        return env.Undefined();
    }

    // Get the prescaler parameter & convert to uint8_t
    uint8_t prescaler = info[0].As<Napi::Number>().Uint32Value();
    
    // Call the C++ method
    int result = trill->setPrescaler(prescaler);
    
    // Return the result
    return Napi::Number::New(env, result);
   }

    Napi::Value UpdateBaseline(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        
        // Call the C++ method
        int result = trill->updateBaseline();
        
        // Return the result
        return Napi::Number::New(env, result);
    }
};

// This function initializes the TrillWrapper class
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    return TrillWrapper::Init(env, exports);
}

NODE_API_MODULE(trill, Init)
