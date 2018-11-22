# Camera Scanner

This is an Ubuntu Touch App to scan documents using your camera.

Please be aware that this app is still under development and not really working, yet!

## Dependencies
Install [clickable](https://github.com/bhdouglass/clickable), which is used to build this app and dependencies.

This app depends on OpenCV. You can easily build OpenCV by running 

    clickable build-libs

This may take some time, as OpenCV is built for both, armhf and amd64. 

## Installation
To build and install the app, simply run

    clickable

See [clickable documentation](http://clickable.bhdouglass.com/) for details.

To build the app for desktop, run

    clickable desktop

## Credits
The image processing is mainly a port of the Android app [OpenNoteScanner](https://github.com/ctodobom/OpenNoteScanner).
