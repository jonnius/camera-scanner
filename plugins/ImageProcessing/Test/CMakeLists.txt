cmake_minimum_required(VERSION 3.1.0)

project(CameraScannerTest)

add_executable(scannertest Test.cpp ../ONSExtractor.cpp)

set(OpenCV_DIR "../../../build/opencv/x86_64-linux-gnu")
find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(scannertest ${OpenCV_LIBS})
