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
 *          Predicate "position"
 *          Concept "face/smile/salient point"
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

#define HAVE_GUILE
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/value/FloatValue.h>

#include <opencog/guile/SchemePrimitive.h>


#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;
using namespace opencog;

class SingBotVision
{
private:
    AtomSpace *_as;
    thread *run_v;
    
    SingBotVision(AtomSpace *as) {_as = as;};
    static SingBotVision *_singbot_vision;
    
    static void vision_loop(SingBotVision *);
    short init_vision(int cam, int width, int height);
    Mat read_frame();
    vector<Rect> detect_faces(Mat in);
    vector<Rect> detect_smiles(Mat in);
    Point sal_point(Mat in);

    //data
    VideoCapture cap;
    Mat frame;
    const string haarcascade_file_face = "haarcascade_frontalface_alt.xml";
    const string haarcascade_file_smile = "haarcascade_smile.xml";
    CascadeClassifier cascade_face, smile_cascade;
    Ptr<Saliency> sal_det;
    vector<vector<Point> > cntrs;
    vector<Vec4i> hier;
    Point2f cent;
    int largest_cntr_idx = 0;
    double max = 0, area;
    float rad;

public:
    SingBotVision* SingBotVisionGetInstance(AtomSpace *as);
    short start_vision();
    short stop_vision();
};

#endif //_SINGBOT_VISION_PERCEPTION_H_
