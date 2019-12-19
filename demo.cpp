#include "vision/RaspiYolo.hpp"

int main(int argc, char **argv)
{
    RaspiYolo rpy("coco.data", "yolov3-tiny.cfg", "yolov3-tiny.weights");
    while(true)
        printf("...\n");
    return 0;
}
