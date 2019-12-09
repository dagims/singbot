#include "RaspiYolo.hpp"

void RaspiYolo::ipl_into_image(IplImage* src, image im)
{
    unsigned char *data = (unsigned char *)src->imageData;
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    int step = src->widthStep;
    int i, j, k;

    for(i = 0; i < h; ++i){
        for(k= 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
}

RaspiYolo::RaspiYolo(std::string _datacfg, std::string _cfgfile,
                     std::string _weightfile, float _thresh, float hier_thresh,
                     bool _capture, int _cap_width, int _cap_height)
{
    //raspicam::RaspiCam capture;
    //capture.setWidth(320);
    //capture.setHeight(320);
    //capture.setFormat(raspicam::RASPICAM_FORMAT_BGR);
    if(_capture)
    {
        capture.set(CV_CAP_PROP_FRAME_WIDTH, _cap_width);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, _cap_height);
        if ( !capture.open())
        {
            fprintf(stderr, "Error opening camera\n");
            return -1;
        }
    }
    
    datacfg = _datacfg;
    cfgfile = _cfgfile;
    weightfile = _weightfile;
    thresh = _thresh;
    hier_thresh = _hier_thresh;
    *outfile = 0;
    fullscreen = 0;

    options = read_data_cfg(datacfg.c_str());
    name_list = option_find_str(options, "names", "data/names.list");
    names = get_labels(name_list);
    alphabet = load_alphabet();
    net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    srand(2222222);
    nms = 0.45;
    nnp_initialize();
    net->threadpool = pthreadpool_create(4);

    if(_capture)
    {
        cv::Mat img;
        cap.retrieve(img);
        IplImage _img = img;
        image im = make_image;
        ipl_into_image(
    }
}

detections ClassifyFrame(cv::Mat &_in)
{
    IplImage _img = _in;
    image im = make_image(_img.width,
                          _img.height,
                          _img.nChannels);
    ipl_to_image(&_img, im);
    image sized = letterbox_image(im, net->w, net->h);
    time = what_time_is_it_now();
    network_predict(net, sized.data);
    printf("Predicted in %f seconds.\n", what_time_is_it_now()-time);
    int nboxes = 0;
    detection *dets = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);
    std::vector<detection> _dets(dets);
    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
    draw_detections(im, dets, nboxes, thresh, names, alphabet, l.classes); 
    free_detections(dets, nboxes);
    if(outfile)
        save_image(im, outfile);

    #ifdef DEBUG_PREVIEW
    cv::namedWindow("predictions", CV_WINDOW_NORMAL);
    show_image(im, "predictions");
    cv::waitKey(0);
    cv::destroyAllWindows();
    #endif
    
    free_image(im);
    free_image(sized);
return _dets;
}

void RaspiYolo::cap_continuous(RaspiYolo *_ry)
{
    while(_ry->isOk())
    {
       cap.grab();
       cap.retrieve(_img);
       detections dets = ClassifyFrame(_img);
    }
}

RaspiYolo::~RaspiYolo()
{
    pthreadpool_destroy(net->threadpool);
    nnp_deinitialize();
    free_network(net);
}
