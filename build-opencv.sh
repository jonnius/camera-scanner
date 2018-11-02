#!/bin/bash

set -e

SOURCE_DIR_BASE_DOCKER=/home/root/camera-scanner
OPENCV_SOURCE_DIR=libs/opencv

DO_CLEAN=0
ARCH="armhf"
ARCH_TRIPLET="arm-linux-gnueabihf"

DOCKER_COMMAND=""
JOBS="1"
SUDO=""

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
    $DOCKER_COMMAND bash <<-EOF
    #!/bin/bash
    export MAKEFLAGS=-j$JOBS
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake -DCMAKE_CROSSCOMPILING=ON -DCMAKE_BUILD_TYPE=Release $SOURCE_DIR_BASE_DOCKER/$OPENCV_SOURCE_DIR
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

    DOCKER_COMMAND="docker run -i -v $PWD:$SOURCE_DIR_BASE_DOCKER -w $SOURCE_DIR_BASE_DOCKER clickable/ubuntu-sdk:16.04-$ARCH"

    if [ "$ARCH" == "host" ]; then
        DOCKER_COMMAND=""
        SUDO="sudo"
    fi

    return 0
}

parse $*

BUILD_DIR=build/tdlib/$ARCH_TRIPLET

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
