#include <functional>
#include <iostream>
#include <sstream>
#include <thread>
#include <string>
#include <node.h>

#include "opencv2/highgui/highgui.hpp"

using namespace v8;

class CameraProperties {
    public:
    int width;
    int height;
    double fps;
    std::string imagePath;
    std::string imagePathTemp;
};

CameraProperties properties;

void CapturingThread() {
    cv::VideoCapture cap(0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, properties.width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, properties.height);
    cap.set(CV_CAP_PROP_FPS, properties.fps);

    cv::Mat frame;

    while (true) {
        bool success = cap.read(frame);

        if (success) {
            cv::imwrite(properties.imagePathTemp, frame);

            std::stringstream ss;
            ss << "mv -f " << properties.imagePathTemp << " " << properties.imagePath;
            system(ss.str().c_str());

            std::cout << "Captured image" << std::endl;
        }
    }
}

void InitMethod(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.Length() > 0) {
        if (!args[0]->IsObject()) {
            Local<Value> exception = Exception::TypeError(String::NewFromUtf8(isolate, "Incorrect argument type"));
            isolate->ThrowException(exception);
            return;
        }

        Local<Object> object = Local<Object>::Cast(args[0]);

        // width
        if (object->Has(String::NewFromUtf8(isolate, "width"))) {
            properties.width = object->Get(String::NewFromUtf8(isolate, "width"))->NumberValue();
        }

        // height
        if (object->Has(String::NewFromUtf8(isolate, "height"))) {
            properties.height = object->Get(String::NewFromUtf8(isolate, "height"))->NumberValue();
        }

        // fps
        if (object->Has(String::NewFromUtf8(isolate, "fps"))) {
            properties.fps = object->Get(String::NewFromUtf8(isolate, "fps"))->NumberValue();
        }

        // image path
        if (object->Has(String::NewFromUtf8(isolate, "imagePath"))) {
            String::Utf8Value imagePath(object->Get(String::NewFromUtf8(isolate, "imagePath"))->ToString());
            properties.imagePath = std::string(*imagePath);
        }

        // image temporary path
        if (object->Has(String::NewFromUtf8(isolate, "imagePathTemp"))) {
            String::Utf8Value imagePathTemp(object->Get(String::NewFromUtf8(isolate, "imagePathTemp"))->ToString());
            properties.imagePathTemp = std::string(*imagePathTemp);
        }
    }

    std::thread thread(CapturingThread);
    thread.detach();
}

void InitModule(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "init", InitMethod);
}

NODE_MODULE(camera, InitModule);
