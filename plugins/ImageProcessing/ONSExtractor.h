#ifndef ONSEXTRACTOR_H
#define ONSEXTRACTOR_H

#include <opencv2/opencv.hpp>

#include "AbstractDocumentExtractor.h"

namespace DocumentScanner
{

/**
 * OpenNoteScanner extractor configuration parameters.
 */
class ONSExtractorConfig
{
public:
    bool colorMode = true;
    bool filterMode = true;
    int colorThr = 110;
    float colorGain = 1.5; // contrast
    float colorBias = 0;   // bright
};

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

	void setConfig(const ONSExtractorConfig & conf)
	{
		m_conf = conf;
	}

private:
	ONSExtractorConfig m_conf;
};
}

#endif
