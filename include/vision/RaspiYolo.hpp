#ifndef _RASPI_YOLO_HPP_
#define _RASPI_YOLO_HPP_

#include <nnpack.h>

extern "C" {
    #include "darknet/darknet.h"
    #include "darknet/utils.h"
    #include <pthreadpool.h>
};


#include <raspicam/raspicam.h>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <mutex>

class RaspiYolo {
private:
    raspicam::RaspiCam_Cv cap;
    int cap_width;
    int cap_height;
    std::thread *run;
    bool ok = false;
    cv::Mat _img;
    
    std::string datacfg;
    std::string cfgfile;
    std::string weightfile;
    float thresh;
    float hier_thresh;
    char *outfile;
    int fullscreen;
    list *options;
    char *name_list;
    char **names;
    int classes;
    image **alphabet;
    network *net;
    double _time;
    char *buff;
    float nms;

    static void cap_continuous(RaspiYolo *_ry);
    void ipl_to_image(IplImage* src, image im);

public:
    RaspiYolo(std::string _datacfg,
              std::string _cfgfile,
              std::string _weightfile,
              float _thresh=0.5,
              float hier_thresh=0.5,
              bool _capture=true,
              int _cap_width=320,
              int _cap_height=320);
    ~RaspiYolo();
    
    bool isOk();
    detection ClassifyFrame(cv::Mat &in);
    std::string get_class_name(int class_id);
};

#endif // _RASPI_YOLO_HPP_
