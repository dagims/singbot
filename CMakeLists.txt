cmake_minimum_required(VERSION 3.0)
project(singbot)
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake"
    "/usr/local/lib/cmake/")
set(CMAKE_CXX_STANDARD 11)
find_package(raspicam REQUIRED)
find_package(Guile REQUIRED)
find_package(NNPACK REQUIRED)
find_package(AtomSpaca QUIET)
find_package(OpenCV REQUIRED)

configure_file(
    data/haarcascade/haarcascade_frontalface_alt.xml
    haarcascade_fontalface_alt.xml
    COPYONLY)
configure_file(
    data/haarcascade/haarcascade_smile.xml
    haarcascade_smile.xml
    COPYONLY)
configure_file(
    data/yolo/coco.data
    coco.data
    COPYONLY)
configure_file(
    data/yolo/yolov3-tiny.cfg
    yolov3-tiny.cfg
    COPYONLY)
configure_file(
    data/yolo/yolov3-tiny.weights
    yolov3-tiny.weights
    COPYONLY)

add_definitions(-DNNPACK -DNNPACK_FAST -DARM_NEON -DOPENCV)
set(DARKNET_CFLAGS -Wall -Wno-unknown-pragmas -Wfatal-errors -fPIC -march=native -O3 -mfpu=neon-vfpv4 -funsafe-math-optimizations -ftree-vectorize)

set(VISION_SRC_FILES
    "${CMAKE_SOURCE_DIR}/lib/vision/RaspiYolo.cpp")
set(VISION_LD
    ${OpenCV_LIBS}
    ${raspicam_CV_LIBS}
    ${NNPACK_LIBRARY}
    pthread
    -lm
    -lpthreadpool)
if(ATOMSPACE_FOUND)
    set(VISION_SRC_FILES
        "${CMAKE_SOURCE_DIR}/lib/vision/SingBotVision.cpp")
endif(ATOMSPACE_FOUND)        


include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/darknet"
    ${GUILE_INCLUDE_DIRS})

add_library(singbot SHARED robo-guile.c)
target_link_libraries(singbot ${GUILE_LIBRARY})

file(GLOB DARKNET_SRC_FILES "${CMAKE_SOURCE_DIR}/lib/darknet/*.c")
add_library(darknet STATIC ${DARKNET_SRC_FILES})
target_compile_options(darknet PRIVATE ${DARKNET_CFLAGS})
target_link_libraries(darknet
    ${NNPACK_LIBRARY}
    ${OpenCV_LIBS}
    pthread
    -lpthreadpool)

add_library(
    singbotvision
    STATIC
    ${VISION_SRC_FILES})
target_link_libraries(
    singbotvision
    darknet
    ${VISION_LD})
add_definitions(-DDEBUG_PREVIEW)
add_executable(demo demo.cpp)
target_link_libraries(demo singbotvision)
#add_executable(robo-test robotest.c)
