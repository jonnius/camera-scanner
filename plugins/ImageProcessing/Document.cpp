#include "Document.h"

using namespace DocumentScanner;
using namespace cv;

Document::Document(const Mat & rawImg,
                   const AbstractDocumentExtractor &extractor)
{
    rawImg.copyTo(m_rawImg);
    m_docImg = Mat();
    m_docExtracted = extractor.extractDocument(m_rawImg, m_docImg);
}

Document::Document(const Mat & rawImg,
                   const Mat & docImg,
                   bool docExtracted /*= true*/) : m_docExtracted(docExtracted)
{
    rawImg.copyTo(m_rawImg);
    if (docExtracted)
        docImg.copyTo(m_docImg);
    else
        m_docImg = Mat();
}

Mat Document::getRawImage() const
{
    return m_rawImg;
}

Mat Document::getDocImage() const
{
    return m_docExtracted ? m_docImg : m_rawImg;
}

bool Document::docExtracted() const
{
    return m_docExtracted;
}
