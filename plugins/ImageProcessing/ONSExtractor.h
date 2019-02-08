#ifndef ONSEXTRACTOR_H
#define ONSEXTRACTOR_H

#include <opencv2/opencv.hpp>

#include "AbstractDocumentExtractor.h"

namespace DocumentScanner
{

/**
  * The OpenNoteScanner extractor is a reimplementation of the document
  * extraction algorithm used in the app OpenNoteScanner by
  * Claudemir Todo Bom (https://github.com/ctodobom/OpenNoteScanner).
  */
class ONSExtractor : public AbstractDocumentExtractor
{
public:
    bool extractDocument(const cv::Mat & rawImg,
                         cv::Mat & docImg) const;
};
}

#endif
