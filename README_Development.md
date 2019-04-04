# Camera Scanner - An Ubuntu Touch Document Scanner App

## Test Image Processing only
Compile OpenCV for desktop as described in the [README](README.md). Additionally install the following packages:

    sudo apt install libjpeg-dev libpng-dev libtiff-dev

Uncomment line 6 in **plugins/ImageProcessing/Debugger.h**:

    #define NOQT

Now you can code into **plugins/ImageProcessing/Test/Test.cpp** whatever you want. Compile and run it:

    cd plugins/ImageProcessing/Test
    cmake .
    make
    ./scannertest
