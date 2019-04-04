#define NOQT
#include "../ONSExtractor.h"

#include <iostream>

using namespace cv;
using namespace std;
using namespace DocumentScanner;

int main()
{
    Mat image = imread("test.jpg");

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    Mat doc;
    ONSExtractor extractor;
    if (!extractor.extractDocument(image, doc))
    {
        cout <<  "Could not detect document" << std::endl ;
        return -1;
    }

    imwrite( "testouput.jpg", doc );


    return 0;
}
