# Camera Scanner - An Ubuntu Touch Document Scanner App

This is an Ubuntu Touch App to scan documents using your camera.

## Dependencies
Install [clickable](https://github.com/bhdouglass/clickable), which is used to build this app and dependencies.

This app depends on OpenCV. You can easily build OpenCV by running

    git submodule update --init --recursive
    clickable build-libs
    clickable build-libs --arch amd64 # If you want to do desktop builds, too

This may take quite some time, but only needs to be done once.

## Installation
To build and launch the app, simply run

    clickable
    clickable desktop # to try a desktop build

See [clickable documentation](http://clickable.bhdouglass.com/) for details.

## Contributors
* Jonatan Hatakeyama Zeidler (jonnius)
* Joan CiberSheep (cibersheep)
* Christian Pauly (krille)

The image processing algorithm is based on the one used in [OpenNoteScanner](https://github.com/ctodobom/OpenNoteScanner).
