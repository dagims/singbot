/*
 * File: vision-perception.hpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: August, 2019
*/

/*
 * Guile Scheme bindings for some vision tools from OpenCV
 * Only has camera capture, face, smile and saliency detection.
 *
 * Runs on its own thread and sets opencog values on detection.
 * Values set according to:
 *      Valuation
 *          Concept "face/smile/salient point"
 *          Predicate "position"
 *          FloatValue X Y
 * The X,Y values are normalized between 0 - 1 where 0,0 is 
 * top left corner of frame.
 *
 * Currently only detecting one of each. If multiple faces in
 * view, only one of them is tracked.
*/

#ifndef _SINGBOT_VISION_PERCEPTION_H_
#define _SINGBOT_VISION_PERCEPTION_H_

#include <opencv2/opencv.hpp>
#include <opencv2/saliency.hpp>

#define HAVE_GUILE
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/value/FloatValue.h>

#include <opencog/guile/SchemePrimitive.h>

#include "vision/RaspiYolo.hpp"

#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <mutex>

#define SCALE_D(x, n) (x / float(n))

using namespace std;
using namespace cv;
using namespace cv::saliency;
using namespace opencog;

class SingBotVision
{
private:
    AtomSpace *_as;
    thread *run_v;
    
    RaspiYolo *rpy;
    detection yolo_dets;

    static void vision_loop(SingBotVision *);
    Mat read_frame();
    vector<Rect> detect_faces(Mat in);
    vector<Rect> detect_smiles(Mat in);
    Point2f detect_sal(Mat in);

    //data
    int cam, width, height;
    VideoCapture cap;
    Mat frame;
    const string haarcascade_file_face = "haarcascade_frontalface_alt.xml";
    const string haarcascade_file_smile = "haarcascade_smile.xml";
    CascadeClassifier cascade_face, cascade_smile;
    Ptr<Saliency> sal_det;
    vector<vector<Point> > cntrs;
    vector<Vec4i> hier;
    Point2f cent;
    int largest_cntr_idx = 0;
    double max = 0, area;
    float rad;
    bool ok = false;


public:
    SingBotVision();
    
    void init_as(AtomSpace *as) {_as = as; ok = true;}
    void init_vision();
    void start_vision();
    void stop_vision();
};

#endif //_SINGBOT_VISION_PERCEPTION_H_
