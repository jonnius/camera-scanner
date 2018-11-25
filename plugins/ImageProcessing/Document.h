#ifndef Document_H
#define Document_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace DocumentScanner
{

typedef std::vector<cv::Point> Contour;

/**
  * CQuadrilateral is a data structure containg contour and
  * quadrilateral of the detected document inside the image.
  */
class CQuadrilateral
{
public:
	CQuadrilateral()
	{
		m_initialized = false;
	}

  CQuadrilateral(Contour contour, std::vector<cv::Point> points) :
  	  m_contour(contour), m_points(points)
  {
    m_initialized = true;
  }

  bool initialized()
  {
  	return m_initialized;
  }

  /**
    * Release memory by deleting points.
    */
  void release()
  {
    m_initialized = false;
    m_points.clear();
  	m_contour.clear();
  }

  Contour getContour()
  {
  	return m_contour;
  }

  std::vector<cv::Point> getPoints()
	{
  	return m_points;
	}

private:
	Contour m_contour;
  std::vector<cv::Point> m_points;
  bool m_initialized;
};

/**
  * Document represents one document and its detection inside
  * an image. It is initialized by specifying an image and will
  * detect and cut out the document, rectify and enhance it. 
  */
class Document
{
public:
  Document() : m_success(false)
  {
    /* empty */
  }

  Document(const cv::Mat & image) : m_success(false),
                                    m_original(image),
                                    m_processed(image)
  {
  	/* empty */
  }

  void detectDocument();

  cv::Mat getProcessed() {
      return m_processed;
  }
  
  /**
    * Release memory. This will let the object in an uninitialized
    * state. Use detectDocument() to process a new image.
    */
  void release()
  {
  	m_processed.release();
  	m_original.release();
    m_quadrilateral.release();
  }

  std::vector<cv::Point> getContour()
  {
    return m_quadrilateral.getContour();
  }

  std::vector<cv::Point> getQuadrilateral()
  {
    return m_quadrilateral.getPoints();
  }

  cv::Mat getDocImage()
  {
    return m_processed;
  }

  cv::Mat getRawImage()
  {
    return m_original;
  }
  
  bool docDetected()
  {
	  return m_success;
  }

private:
  cv::Mat m_original;
  cv::Mat m_processed;
  CQuadrilateral m_quadrilateral;
  std::vector<cv::Point> m_previewPoints; // TODO Sinn prüfen
  cv::Size m_previewSize;
  bool m_success;
};
}

#endif
