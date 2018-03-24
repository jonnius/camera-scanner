#ifndef CDOCUMENT_H
#define CDOCUMENT_H

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace DocumentScanner
{

typedef std::vector<cv::Point> Contour;

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

class CDocument
{
public:
  CDocument()
  {
    /* empty */
  }

  CDocument(const cv::Mat & image)
  {
  	detectDocument(image);
  }

  void detectDocument(const cv::Mat & inputRgba);

  cv::Mat getProcessed() {
      return m_processed;
  }

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

  cv::Mat getResult()
  {
    return m_processed;
  }

private:
	cv::Mat m_original;
  cv::Mat m_processed;
  CQuadrilateral m_quadrilateral;
  std::vector<cv::Point> m_previewPoints; // TODO Sinn prüfen
  cv::Size m_previewSize;
};
}

#endif
