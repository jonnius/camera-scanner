#!/bin/bash

set -e

SOURCE_DIR_BASE=$PWD
SOURCE_DIR_BASE_DOCKER=/home/root/camera-scanner
OPENCV_SOURCE_DIR=libs/opencv

DO_CLEAN=0
ARCH="armhf"
ARCH_TRIPLET="arm-linux-gnueabihf"

DOCKER_COMMAND=""
JOBS="1"

# OpenCV Build args
OPENCV_BUILD_ARGS="-DCMAKE_BUILD_TYPE=Release"
# Only build the modules that we need
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DBUILD_LIST=core,imgproc,highgui,imgcodecs"
# Disable some stuff to reduce build time
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DBUILD_EXAMPLES=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DBUILD_DOCS=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DBUILD_PERF_TESTS=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DBUILD_TESTS=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DBUILD_OPENCV_APPS=OFF"
# Enable some stuff to speed it up
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DWITH_TBB=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DWITH_OPENMP=OFF" # we should try if this speeds up the image processing
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DWITH_IPP=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DWITH_NVCUVID=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DWITH_CUDA=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DWITH_CSTRIPES=OFF"
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DWITH_OPENCL=OFF"
# Static libs
OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DBUILD_SHARED_LIBS=OFF"
# Debug stuff
#OPENCV_BUILD_ARGS="$OPENCV_BUILD_ARGS -DENABLE_PROFILING=ON" #I guess it helps debugging

function usage
{
    echo "usage: $0 [arch] [clean]"
}

function clean
{
    $DOCKER_COMMAND bash <<-EOF
    #!/bin/bash
    rm -rf $BUILD_DIR
EOF
}

function build
{
    git submodule update --init --recursive
    mkdir -p $BUILD_DIR
    $DOCKER_COMMAND bash <<-EOF
    #!/bin/bash
    export MAKEFLAGS=-j$JOBS
    cd $BUILD_DIR
    cmake $OPENCV_BUILD_ARGS $SOURCE_DIR
    cmake --build .
EOF
}

function parse
{
    if [ $# -gt 2 ]; then
        echo "Too many parameters..."
        return 1
    fi

    if [ $# -eq 2 -a "$1" != "clean" -a "$2" != "clean" ]; then
        return 1
    fi

    for param in $*; do
        case $param in
            "clean")
                DO_CLEAN=1
                ;;
            "amd64")
                ARCH="amd64"
                ARCH_TRIPLET="x86_64-linux-gnu"
                ;;
            "armhf")
                ARCH="armhf"
                ARCH_TRIPLET="arm-linux-gnueabihf"
                ;;
            "host")
                ARCH="host"
                ARCH_TRIPLET=$(dpkg-architecture -q DEB_HOST_MULTIARCH)
                ;;

            *)
                return 1
        esac
    done

    DOCKER_COMMAND="docker run -i -v $SOURCE_DIR_BASE:$SOURCE_DIR_BASE_DOCKER -w $SOURCE_DIR_BASE_DOCKER clickable/ubuntu-sdk:16.04-$ARCH"
    SOURCE_DIR=$SOURCE_DIR_BASE_DOCKER/$OPENCV_SOURCE_DIR

    if [ "$ARCH" == "host" ]; then
        DOCKER_COMMAND=""
        SOURCE_DIR=$SOURCE_DIR_BASE/$OPENCV_SOURCE_DIR
    fi

    return 0
}

parse $*

BUILD_DIR=build/opencv/$ARCH_TRIPLET

if [ $? -eq 1 ]; then
    usage
    exit 1
fi

if [ $DO_CLEAN -eq 1 ]; then
    echo "Cleaning..."
    clean
    echo "Done..."
else
    echo "Building..."
    build
    echo "Done..."
fi
