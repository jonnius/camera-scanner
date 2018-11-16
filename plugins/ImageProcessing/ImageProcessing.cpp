#include <QDebug>
#include <QtGui/QImage>

#include "ImageProcessing.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "highgui.h"

#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;

ImageProcessing::ImageProcessing() {
  /* empty */
}

QImage ImageProcessing::processImage(const QImage & image)
{
  qDebug() << "starting to process image...";
  QImage imgcopy = image.copy();
  m_document.detectDocument(Mat(imgcopy.height(),
                                imgcopy.width(),
                                CV_8UC3,
                                imgcopy.bits(),
                                imgcopy.bytesPerLine()));

  qDebug() << "finished image processing...";
  Mat result = m_document.getResult();
  return QImage((uchar*) result.data, result.cols, result.rows, result.step, QImage::Format_RGB888);
}

//std::vector<cv::Point> ImageProcessing::getContour()
//{
//  return m_document.getContour();
//}

//std::vector<cv::Point> ImageProcessing::getQuadrilateral()
//{
//  return m_document.getQuadrilateral();
//}

//cv::Mat ImageProcessing::getResult()
//{
//  return m_document.getResult();
//}
