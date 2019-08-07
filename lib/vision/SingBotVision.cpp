/*
 * File: SingBotVision.cpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: August, 2019
*/

SingBotVision *SingBotVision::_singbot_vision = NULL;

SingBotVision *SingBotVision::SingBotVisionGetInstance(AtomSpace *as)
{
    if(_singbot_vision == NULL)
        _singbot_vision = new SingBotVision(as);
    return _singbot_vision;
}

void SingBotVision::start_vision()
{
    //TODO check if thread not already running
    run = new thread(SingBotVision::vision_loop, this);
}

void SingBotVision::stop_vision()
{
    ok = false;
    run->join();
    delete run;
}

short SingBotVision::init_vision(int cam, int width, int height)
{
    cap.open(cam);
    if(!cap.isOpened())
        return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    cascade_face.load(haarcascade_file_face);
    cascade_smile.load(haarcascade_file_smile);
    return 0;
}

Mat SingBotVision::read_frame()
{
    cap >> frame;
    return frame.clone();
}

vector<Rect> detect_faces(Mat in)
{
    vector<Rect> out;
    cascade_face.detectMultiScale(in, out, 1.1, 4, 
                    0|CV_HAAR_SCALE_IMAGE, Size(55, 55), 
                    Size(200, 200));
    return out;
}

vector<Rect> detect_smiles(Mat in)
{
    vector<Rect> out;
    cascade_smile.detectMultiScale(in, out,1.7, 8,
                    0|CV_HAAR_SCALE_IMAGE, Size(20, 20) );
    return out;
}


void SingBotVision::vision_loop(SingBotVision *_sbv)
{
    //call read frame, face detect, smile detect and sal detect
}
