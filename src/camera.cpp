#include <node.h>
#include <iostream>
#include <thread>
#include <functional>

#include "opencv2/highgui/highgui.hpp"

using namespace v8;

class CameraProperties {
    private:
    const int DEFAULT_WIDTH = 320;
    const int DEFAULT_HEIGHT = 240;
    const double DEFAULT_FPS = 0.5;

    public:
    int width;
    int height;
    double fps;

    CameraProperties():
        width(DEFAULT_WIDTH),
        height(DEFAULT_HEIGHT),
        fps(DEFAULT_FPS)
        {}
};

CameraProperties properties;

void CapturingThread() {
    std::cout << "Camera properties..." << std::endl;
    std::cout << "width: " << properties.width << std::endl;
    std::cout << "height: " << properties.height << std::endl;
    std::cout << "fps: " << properties.fps << std::endl;
    std::cout << "Starting." << std::endl;

    cv::VideoCapture cap(0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, properties.width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, properties.height);
    cap.set(CV_CAP_PROP_FPS, properties.fps);

    cv::Mat frame;

    while (true) {
        bool success = cap.read(frame);

        if (success) {
            cv::imwrite("/dev/shm/image.temp.jpg", frame);
            system("mv -f /dev/shm/image.temp.jpg /dev/shm/image.jpg");
            std::cout << "captured" << std::endl;
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
    }

    std::cout << "camera properties" << std::endl;

    std::thread thread(CapturingThread);
    thread.detach();
}

void InitModule(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "init", InitMethod);
}

NODE_MODULE(camera, InitModule);
