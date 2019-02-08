#ifndef ABSTRACTDOCUMENTEXTRACTOR_H
#define ABSTRACTDOCUMENTEXTRACTOR_H

#include <opencv2/opencv.hpp>

namespace DocumentScanner
{

/**
  * This is an interface to image processing classes, that are able to
  * detect an process a document page in an image.
  */
class AbstractDocumentExtractor
{
public:
    virtual bool extractDocument(const cv::Mat & rawImg,
                                 cv::Mat & docImg) const = 0;
};
}

#endif
