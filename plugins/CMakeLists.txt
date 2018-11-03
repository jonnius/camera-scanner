find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})
add_subdirectory(ImageProcessing)
