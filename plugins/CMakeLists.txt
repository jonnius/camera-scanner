# OpenCV
#option(BUILD_OPENCV "enable/disable" ON)

## Give it its own build directory
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../opencv_build/)

## Include paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/opencv/include)
### Some headers get configured and put into the bin dir by CMake
include_directories(${CMAKE_BINARY_DIR})
### Add necessary modules to include dir
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/opencv/modules/core/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/opencv/modules/highgui/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/opencv/modules/imgproc/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/opencv/modules/imgcodecs/include)

## CMake Module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/opencv)
# We should disable all modules that we don't need
set(BUILD_LIST "core,imgproc,highgui,imgcodecs" CACHE STRING "Only use core, imgproc, highgui and imgcodecs modules")
#set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING "Release configuration")
# Disable some stuff to reduce build time
set(BUILD_EXAMPLES OFF CACHE BOOL "No need for examples")
set(BUILD_DOCS OFF CACHE BOOL "No need for docs")
set(BUILD_PERF_TESTS OFF CACHE BOOL "No need for tests")
set(BUILD_TESTS OFF CACHE BOOL "No need for tests")
set(BUILD_OPENCV_APPS OFF CACHE BOOL "Disable OpenCV apps")
# Enable some stuff to speed it up (just found that somewhere)
set(WITH_TBB OFF CACHE BOOL "No idea what this does")
set(WITH_OPENMP OFF CACHE BOOL "No idea if parallelization will work on the phone")
set(WITH_IPP OFF CACHE BOOL "No idea what this does")
set(WITH_NVCUVID OFF CACHE BOOL "No idea what this does")
set(WITH_CUDA OFF CACHE BOOL "No idea if it would work on the phone")
set(WITH_CSTRIPES OFF CACHE BOOL "No idea what this does")
set(WITH_OPENCL OFF CACHE BOOL "No idea what this does")
# Debug stuff
set(ENABLE_PROFILING ON CACHE BOOL "I guess it helps debugging")
set(CMAKE_BUILD_TYPE "DEBUG" CACHE STRING "Debug configuration")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Shared libs seem to complicate debugging")

## Compile OpenCV
#if(DEFINED ENV{BUILD_OPENCV})
#  message("BUILD_OPENCV ist definiert")
#else()
#  message("BUILD_OPENCV ist nicht definiert")
#endif()
add_subdirectory(opencv)
#set(Test $ENV{WINDOWPATH})

#if(${BUILD_OPENCV})
#  message("OpenCV bauen")
#else()
#  message("OpenCV nicht bauen")
#endif() 

#if($ENV{SKIP_OPENCV_BUILD})
#  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/opencv)
#else()
#  message(${Test})
#  message("Skipping OpenCV build...")
#endif()


add_subdirectory(ImageProcessing)
