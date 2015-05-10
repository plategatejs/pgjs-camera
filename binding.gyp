{
    "targets": [{
        "target_name": "camera",
        "sources": ["src/camera.cpp"],
        "cflags_cc": ["-std=c++11"],

        "include_dirs": ["/usr/include/opencv2"],

        "link_settings": {
            "libraries": ["-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video"],
            "library_dirs": ["/usr/lib"],
        },
    }]
}