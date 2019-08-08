cmake_minimum_required(VERSION 3.0)
project(singbot)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 11)
find_package(Guile REQUIRED)
find_package(AtomSpace REQUIRED)
find_package(OpenCV REQUIRED)

configure_file(
    data/haarcascade/haarcascade_fontalface_alt.xml
    haarcascade_fontalface_alt.xml
    COPYONLY)
configure_file(
    data/haarcascade/haarcascade_smile.xml
    haarcascade_smile.xml
    COPYONLY)

include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    ${GUILE_INCLUDE_DIRS})

add_library(singbot SHARED robo-guile.c)
target_link_libraries(singbot ${GUILE_LIBRARY})

add_library(
    singbotvision
    SHARED
    ${CMAKE_SOURCE_DIR}/lib/vision/SingBotVision.cpp)
target_link_libraries(
    singbotvision
    ${GUILE_LIBRARY}
    ${ATOMSPACE_LIBRARIES}
    ${OpenCV_LIBS}
    pthread)

#add_executable(robo-test robotest.c)
