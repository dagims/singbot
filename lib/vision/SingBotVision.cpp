/*
 * File: SingBotVision.cpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: August, 2019
*/

#include "vision/SingBotVision.hpp"

extern "C" {
    void init_vision_binding();
}

void SingBotVision::start_vision()
{
    //TODO check if thread not already running
    run_v = new thread(SingBotVision::vision_loop, this);
}

void SingBotVision::stop_vision()
{
    ok = false;
    run_v->join();
    delete run_v;
}

void SingBotVision::init_vision()
{
    //XXX these values should be set from the scheme module
    cam = 0;
    width = 320;
    height = 320;
    cap.open(cam);
    if(!cap.isOpened())
        return;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    cascade_face.load(haarcascade_file_face);
    cascade_smile.load(haarcascade_file_smile);
    rpy = new RaspiYolo("coco.data", "yolov3-tiny.cfg",
                        "yolov3-tiny.weights", 0.5, 0.5, false);
}

Mat SingBotVision::read_frame()
{
    cap >> frame;
    return frame.clone();
}

vector<Rect> SingBotVision::detect_faces(Mat in)
{
    vector<Rect> out;
    cascade_face.detectMultiScale(in, out, 1.1, 4, 
                    0|CV_HAAR_SCALE_IMAGE, Size(55, 55), 
                    Size(200, 200));
    return out;
}

vector<Rect> SingBotVision::detect_smiles(Mat in)
{
    vector<Rect> out;
    cascade_smile.detectMultiScale(in, out,1.7, 8,
                    0|CV_HAAR_SCALE_IMAGE, Size(20, 20) );
    return out;
}

Point2f SingBotVision::detect_sal(Mat in)
{
    sal_det = StaticSaliencyFineGrained::create();
    cent.x = 0; cent.y = 0;
    Mat out; //XXX remove this
    if(sal_det->computeSaliency(in, out)) {
        threshold(out, out, 100, 255, CV_THRESH_BINARY);
        findContours(out, cntrs, hier, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        if(cntrs.size() > 0) {
            largest_cntr_idx = 0;
            max = 0;
            for(size_t idx = 0; idx < cntrs.size(); idx++) {
                area = contourArea(cntrs[idx]);
                if (area > max) {
                    max = area;
                    largest_cntr_idx = idx;
                }
            }
            minEnclosingCircle(cntrs[largest_cntr_idx], cent, rad);
        }
    }
    return cent;
}

void SingBotVision::vision_loop(SingBotVision *_sbv)
{
    /*
     * Values are created in the following format for use by the BT
     * 
     * Faces and Smiles
     *   When a face is detected, a ConceptNode will be create as
     *   (ConceptNode "face_0") and face_1 for the second face and
     *   and so on. The position of each face will be indicated as
     *   shown here:
     *     Value:
     *       Concept "face_x"
     *       Predicate "position"
     *         Float x y
     *   The smiles will be indicated per face as shown here:
     *     Value:
     *       Concept "face_x"
     *       Predicate "smile"
     *         Float 1.0
     *   Where 1.0 means the face is smiling and 0.0 means not smiling.
     *
     * Salient Point
     *   Salient point will be indicated with its x,y coordinate
     *     Value:
     *       Concept "sal_point"
     *       Predicate "position"
     *         Float x y
     * 
    */
    
    Mat img, fimg, yimg;
    vector<Rect> faces, smiles;
    Point2f cent;
    string str;
    size_t idx = 0;

    Handle h;
    Handle pr_h = _sbv->_as->add_node(CONCEPT_NODE, "person");
    Handle dg_h = _sbv->_as->add_node(CONCEPT_NODE, "dog");
    Handle ps_h = _sbv->_as->add_node(PREDICATE_NODE, "position");
    Handle sm_h = _sbv->_as->add_node(PREDICATE_NODE, "smiling");
    Handle sp_h = _sbv->_as->add_node(CONCEPT_NODE, "sal_point");
    Handle sn_h = _sbv->_as->add_node(CONCEPT_NODE, "sense");
    Handle rt_h = _sbv->_as->add_node(CONCEPT_NODE, "rate");

    ValuePtr pap;

    while(_sbv->ok){
        img = _sbv->read_frame();
        cvtColor(img, yimg, COLOR_BGR2RGB);
        cvtColor(img, fimg, COLOR_BGR2GRAY);
        equalizeHist(fimg.clone(), fimg);
        faces = _sbv->detect_faces(fimg);
        _sbv->yolo_dets = _sbv->rpy->ClassifyFrame(yimg);
        std::cout<<"**** YOLO Detections **** = "<<_sbv->yolo_dets.classes<<"\n";
        for(idx = 0; idx < faces.size(); idx++) {
            str = "face_" + std::to_string(idx);
            h = _sbv->_as->add_node(CONCEPT_NODE, str);
            cent.x = faces[idx].x + (faces[idx].width/2.0);
            cent.y = faces[idx].y + (faces[idx].height/2.0);
            pap = createFloatValue(
                    vector<double>({
                        SCALE_D(cent.x, _sbv->width),
                        SCALE_D(cent.y, _sbv->height)}));
            h->setValue(ps_h, pap);
            smiles = _sbv->detect_smiles(fimg(faces[idx]));
            pap = createFloatValue(smiles.empty() ? 0.0 : 1.0);
            h->setValue(sm_h, pap);
            cent = _sbv->detect_sal(fimg);
            pap = createFloatValue(
                    vector<double>({
                        SCALE_D(cent.x, _sbv->width),
                        SCALE_D(cent.y, _sbv->height)}));
            sp_h->setValue(ps_h, pap);
        }

    }
}

void init_vision_binding()
{
    SingBotVision *singbotv = new SingBotVision();
    define_scheme_primitive("c-init-as", &SingBotVision::init_as, singbotv);
    define_scheme_primitive("c-init-vision", &SingBotVision::init_vision, singbotv);
    define_scheme_primitive("c-start-vision", &SingBotVision::start_vision, singbotv);
    define_scheme_primitive("c-stop-vision", &SingBotVision::stop_vision, singbotv);
}
