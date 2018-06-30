## Include paths
include_directories(opencv/include)
# OpenCV puts platform depended headers into the binary dir 
include_directories(${CMAKE_BINARY_DIR})
### Add necessary modules to include dir
include_directories(opencv/modules/core/include)
include_directories(opencv/modules/highgui/include)
include_directories(opencv/modules/imgproc/include)
include_directories(opencv/modules/imgcodecs/include)

## CMake Module path
list(APPEND CMAKE_MODULE_PATH opencv)
# We should only enable modules that we need
set(BUILD_LIST "core,imgproc,highgui,imgcodecs" CACHE STRING "Only use core, imgproc, highgui and imgcodecs modules")
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
# If we plan a release:
#set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING "Release configuration")

## Compile OpenCV
add_subdirectory(opencv)

# Our own ImageProcessing
add_subdirectory(ImageProcessing)
