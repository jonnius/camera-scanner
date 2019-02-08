#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <opencv2/opencv.hpp>

#include "AbstractDocumentExtractor.h"

namespace DocumentScanner
{

/**
  * The Document class holds the raw image of a document page, the
  * information whether a document has been sucessfully extracted and if
  * so, the processed document image.
  */
class Document
{
public:
    /**
     * Set the raw image and let the specified extractor produce the
     * document image.
     */
    Document(const cv::Mat & rawImg,
             const AbstractDocumentExtractor &extractor);

    /**
     * Set the raw image and the processed document image. If
     * docExtracted is false, docImg is ignored.
     */
    Document(const cv::Mat & rawImg,
             const cv::Mat & docImg,
             bool docExtracted = true);

    cv::Mat getRawImage();
    /**
     * Returns the processed document image. If the document
     * extraction has failed, the raw image is returned instead.
     */
    cv::Mat getDocImage();
    bool docExtracted();

private:
    cv::Mat m_rawImg;
    cv::Mat m_docImg;
    bool m_docExtracted;
};
}

#endif
