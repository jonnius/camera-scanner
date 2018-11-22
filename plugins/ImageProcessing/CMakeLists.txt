set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PLUGIN "ImageProcessing")

# Find OpenCV
set(OpenCV_DIR "${CMAKE_BINARY_DIR}/../opencv/${ARCH_TRIPLET}")
find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})

set(
    SRC
    plugin.cpp
    ImageProcessing.cpp
    Document.cpp
    DocumentStore.cpp
)

set(CMAKE_AUTOMOC ON)

add_library(${PLUGIN} MODULE ${SRC})
set_target_properties(${PLUGIN} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PLUGIN})
qt5_use_modules(${PLUGIN} Qml Quick DBus)
target_link_libraries(${PLUGIN} ${OpenCV_LIBS})

execute_process(
    COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
    OUTPUT_VARIABLE ARCH_TRIPLET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")

install(TARGETS ${PLUGIN} DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/${PLUGIN}/)
